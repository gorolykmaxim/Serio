package org.serio.core.showscrawler;

import org.apache.commons.lang3.StringUtils;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Captor;
import org.mockito.MockitoAnnotations;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;

import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.Assert.*;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.*;

public abstract class BaseCrawlingTest extends BaseShowsCrawlerTest {
    protected CrawlingMethod method;
    @Captor
    protected ArgumentCaptor<List<CrawlLogEntry>> logCaptor;

    @Before
    public void init() {
        MockitoAnnotations.initMocks(this);
    }
    
    @Test
    public void shouldCrawlAndSaveInStorageCompleteCrawler() {
        // given
        String showCrawler = String.format(COMPLETE_SHOW_CRAWLER_TEMPLATE, SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
        // when
        method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
        // then
        ArgumentCaptor<String> showCrawlerCaptor = ArgumentCaptor.forClass(String.class);
        verify(showCrawlerStorage).saveShowCrawler(eq(showId), showCrawlerCaptor.capture());
        assertEquals(StringUtils.deleteWhitespace(showCrawler), StringUtils.deleteWhitespace(showCrawlerCaptor.getValue()));
    }

    @Test
    public void shouldCrawlAndSaveInStorageBareMinimumCrawler() {
        // given
        String showCrawler = String.format(COMPLETE_SHOW_CRAWLER_TEMPLATE, SHOW_NAME, CRAWLER_WITH_EMPTY_STEPS, COMPLETE_VIDEO_CRAWLER, CRAWLER_WITH_EMPTY_STEPS);
        // when
        method.call(SHOW_NAME, null, COMPLETE_VIDEO_CRAWLER, null);
        // then
        ArgumentCaptor<String> showCrawlerCaptor = ArgumentCaptor.forClass(String.class);
        verify(showCrawlerStorage).saveShowCrawler(eq(showId), showCrawlerCaptor.capture());
        assertEquals(StringUtils.deleteWhitespace(showCrawler), StringUtils.deleteWhitespace(showCrawlerCaptor.getValue()));
    }

    @Test
    public void shouldCrawlAndSaveInStorageAnyCrawlerEvenIfLogSavingFails() {
        // given
        doThrow(new RuntimeException()).when(showCrawlerLogStorage).saveCrawlingLogOfTheShow(any(), any());
        String showCrawler = String.format(COMPLETE_SHOW_CRAWLER_TEMPLATE, SHOW_NAME, CRAWLER_WITH_EMPTY_STEPS, COMPLETE_VIDEO_CRAWLER, CRAWLER_WITH_EMPTY_STEPS);
        // when
        try {
            method.call(SHOW_NAME, null, COMPLETE_VIDEO_CRAWLER, null);
        } catch (ShowCrawlerException ignored) {}
        // then
        ArgumentCaptor<String> showCrawlerCaptor = ArgumentCaptor.forClass(String.class);
        verify(showCrawlerStorage).saveShowCrawler(eq(showId), showCrawlerCaptor.capture());
        assertEquals(StringUtils.deleteWhitespace(showCrawler), StringUtils.deleteWhitespace(showCrawlerCaptor.getValue()));
    }

    @Test
    public void shouldCrawlAndSaveLogOfCompleteCrawler() {
        // when
        method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
        // then
        verify(showCrawlerLogStorage).saveCrawlingLogOfTheShow(eq(showId), logCaptor.capture());
        String log = logCaptor.getValue().toString();
        assertTrue(log.contains("Crawling thumbnail"));
        assertTrue(log.contains("Crawling episode videos"));
        assertTrue(log.contains("Crawling episode names"));
        assertTrue(log.contains(StringUtils.truncate(SHOW_WEB_PAGE, (int) crawlerLogDetailsLength / 2)));
    }

    @Test
    public void shouldCrawlAndSaveLogOfCrawlerWithNoEpisodeNamesCrawler() {
        // when
        method.call(SHOW_NAME, null, COMPLETE_VIDEO_CRAWLER, null);
        // then
        verify(showCrawlerLogStorage).saveCrawlingLogOfTheShow(eq(showId), logCaptor.capture());
        String log = logCaptor.getValue().toString();
        assertTrue(log.contains("Crawling thumbnail"));
        assertTrue(log.contains("Crawling episode videos"));
        assertFalse(log.contains("Crawling episode names"));
        assertTrue(log.contains(StringUtils.truncate(SHOW_WEB_PAGE, (int) crawlerLogDetailsLength / 2)));
    }

    @Test
    public void shouldCrawlShowAndSaveCompleteCrawler() {
        // given
        List<Episode> episodes = IntStream
                .range(1, 11)
                .mapToObj(i -> new Episode(i, String.format("The episode-%d", i), String.format("%s/episode-%d.mp4", SHOW_URL, i)))
                .collect(Collectors.toList());
        Show expectedShow = Show.createNew(SHOW_NAME, SHOW_URL + "/thumbnail.png", episodes);
        // when
        Show show = method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
        // then
        assertEquals(expectedShow, show);
    }

    @Test
    public void shouldCrawlShowAndSaveCompleteCrawlerWithNoThumbnailCrawled() {
        // given
        List<Episode> episodes = IntStream
                .range(1, 11)
                .mapToObj(i -> new Episode(i, String.format("The episode-%d", i), String.format("%s/episode-%d.mp4", SHOW_URL, i)))
                .collect(Collectors.toList());
        Show expectedShow = Show.createNew(SHOW_NAME, episodes);
        // when
        Show show = method.call(SHOW_NAME, CRAWLER_WITH_EMPTY_STEPS, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
        // then
        assertEquals(expectedShow, show);
    }

    @Test
    public void shouldCrawlShowAndSaveCompleteCrawlerWithRegExpWithoutMatchGroups() {
        // given
        List<Episode> episodes = IntStream
                .range(1, 11)
                .mapToObj(i -> new Episode(i, String.format("The episode-%d", i), String.format("%s/episode-%d.mp4", SHOW_URL, i)))
                .collect(Collectors.toList());
        Show expectedShow = Show.createNew(SHOW_NAME, SHOW_URL + "/thumbnail.png", episodes);
        String episodeNameCrawler = String.format(COMPLETE_NAME_CRAWLER_TEMPLATE, "episode-[0-9]+", "The %s");
        // when
        Show show = method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, episodeNameCrawler);
        // then
        assertEquals(expectedShow, show);
    }

    @Test
    public void shouldCrawlShowAndSaveBareMinimumCrawler() {
        // given
        List<Episode> episodes = IntStream
                .range(1, 11)
                .mapToObj(i -> new Episode(i, String.format("%s/episode-%d.mp4", SHOW_URL, i)))
                .collect(Collectors.toList());
        Show expectedShow = Show.createNew(SHOW_NAME, episodes);
        // when
        Show show = method.call(SHOW_NAME, null, COMPLETE_VIDEO_CRAWLER, null);
        // then
        assertEquals(expectedShow, show);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCompleteCrawlerDueToCrawledEpisodeNamesNotMatchingEpisodeVideos() {
        // given
        String episodeNameCrawler = String.format(COMPLETE_NAME_CRAWLER_TEMPLATE, "asd", "%s");
        // when
        method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, episodeNameCrawler);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithoutShowName() {
        // when
        method.call(null, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, COMPLETE_NAME_CRAWLER);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithoutEpisodeVideoCrawler() {
        // when
        method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, null, COMPLETE_NAME_CRAWLER);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithStepWithoutType() {
        // when
        method.call(SHOW_NAME, null, CRAWLER_WITH_STEP_WITHOUT_TYPE, null);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithStepWithUnknownType() {
        // when
        method.call(SHOW_NAME, null, CRAWLER_WITH_STEP_WITH_UNKNOWN_TYPE, null);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithValueStepWithoutValue() {
        // when
        method.call(SHOW_NAME, null, CRAWLER_WITH_VALUE_STEP_WITHOUT_VALUE, null);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithRegExpStepWithoutRegExp() {
        // when
        method.call(SHOW_NAME, null, CRAWLER_WITH_REGEXP_STEP_WITHOUT_REGEXP, null);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerWithTransformStepWithoutTemplate() {
        // when
        method.call(SHOW_NAME, null, CRAWLER_WITH_TRANSFORM_STEP_WITHOUT_TEMPLATE, null);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerDueToHttpClientIOError() {
        // given
        when(httpClient.fetchContentFromUrl(SHOW_URL)).thenReturn(CompletableFuture.supplyAsync(() -> {throw new RuntimeException();}));
        // when
        method.call(SHOW_NAME, null, COMPLETE_VIDEO_CRAWLER, null);
    }

    @Test(expected = ShowCrawlerException.class)
    public void shouldFailToCrawlShowAndSaveCrawlerDueToInvalidRegExp() {
        // given
        String episodeNameCrawler = String.format(COMPLETE_NAME_CRAWLER_TEMPLATE, "(.*))", "The %s");
        // when
        method.call(SHOW_NAME, COMPLETE_THUMBNAIL_CRAWLER, COMPLETE_VIDEO_CRAWLER, episodeNameCrawler);
    }
    
    public interface CrawlingMethod {
        Show call(String showName, String thumbnailCrawler, String episodeVideoCrawler, String episodeNameCrawler);
    }
}
