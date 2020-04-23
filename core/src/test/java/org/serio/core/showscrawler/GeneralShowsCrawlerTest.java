package org.serio.core.showscrawler;

import org.junit.Test;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class GeneralShowsCrawlerTest extends BaseShowsCrawlerTest {
    @Test(expected = ShowCrawlerLookupException.class)
    public void shouldFailToGetSerializedPartsOfShowCrawlerDueToIOError() {
        // given
        when(showCrawlerStorage.findShowCrawlerByShowId(showId))
                .thenReturn(CompletableFuture.supplyAsync(() -> {throw new RuntimeException();}));
        // when
        showsCrawler.getSerializedPartsOfCrawlerOfShow(showId);
    }

    @Test
    public void shouldGetSerializedPartsOfACompleteShowCrawler() {
        // given
        String showCrawler = String.format(COMPLETE_SHOW_CRAWLER_TEMPLATE, SHOW_NAME, CRAWLER_WITHOUT_STEPS, COMPLETE_CRAWLER, CRAWLER_WITH_EMPTY_STEPS);
        when(showCrawlerStorage.findShowCrawlerByShowId(showId)).thenReturn(CompletableFuture.completedFuture(showCrawler));
        // when
        SerializedShowCrawlerParts serializedShowCrawlerParts = showsCrawler.getSerializedPartsOfCrawlerOfShow(showId);
        // then
        assertEquals(SHOW_NAME, serializedShowCrawlerParts.getShowName());
        assertEquals(CRAWLER_WITH_EMPTY_STEPS, serializedShowCrawlerParts.getSerializedThumbnailCrawler());
        assertEquals(COMPLETE_CRAWLER, serializedShowCrawlerParts.getSerializedEpisodeVideoCrawler());
        assertEquals(CRAWLER_WITH_EMPTY_STEPS, serializedShowCrawlerParts.getSerializedEpisodeNameCrawler());
    }

    @Test
    public void shouldGetSerializedPartsOfABareMinimumShowCrawler() {
        // given
        String showCrawler = String.format(BARE_MINIMUM_SHOW_CRAWLER_TEMPLATE, SHOW_NAME, CRAWLER_WITH_EMPTY_STEPS);
        when(showCrawlerStorage.findShowCrawlerByShowId(showId)).thenReturn(CompletableFuture.completedFuture(showCrawler));
        // when
        SerializedShowCrawlerParts serializedShowCrawlerParts = showsCrawler.getSerializedPartsOfCrawlerOfShow(showId);
        // then
        assertEquals(SHOW_NAME, serializedShowCrawlerParts.getShowName());
        assertEquals(CRAWLER_WITH_EMPTY_STEPS, serializedShowCrawlerParts.getSerializedThumbnailCrawler());
        assertEquals(CRAWLER_WITH_EMPTY_STEPS, serializedShowCrawlerParts.getSerializedEpisodeVideoCrawler());
        assertEquals(CRAWLER_WITH_EMPTY_STEPS, serializedShowCrawlerParts.getSerializedEpisodeNameCrawler());
    }

    @Test(expected = ShowCrawlerLookupException.class)
    public void shouldFailToGetSerializedShowCrawlerDueToIOError() {
        // given
        when(showCrawlerStorage.findShowCrawlerByShowId(showId))
                .thenReturn(CompletableFuture.supplyAsync(() -> {throw new RuntimeException();}));
        // when
        showsCrawler.getSerializedCrawlerOfShow(showId);
    }

    @Test
    public void shouldGetSerializedShowCrawler() {
        // given
        String showCrawler = String.format(COMPLETE_SHOW_CRAWLER_TEMPLATE, SHOW_NAME, COMPLETE_CRAWLER, COMPLETE_CRAWLER, COMPLETE_CRAWLER);
        when(showCrawlerStorage.findShowCrawlerByShowId(showId)).thenReturn(CompletableFuture.completedFuture(showCrawler));
        // when
        String serializedShowCrawler = showsCrawler.getSerializedCrawlerOfShow(showId);
        // then
        assertEquals(showCrawler, serializedShowCrawler);
    }

    @Test(expected = CrawlingLogLookupException.class)
    public void shouldFailToGetLastCrawlingLogOfAShowDueToIOError() {
        // given
        when(showCrawlerLogStorage.getLastCrawlingLogOfTheShow(showId))
                .thenReturn(CompletableFuture.supplyAsync(() -> {throw new RuntimeException();}));
        // when
        showsCrawler.getLastCrawlingLogOfShow(showId);
    }

    @Test
    public void shouldGetLastCrawlingLogOfAShow() {
        // given
        List<CrawlLogEntry> expectedLog = new ArrayList<>();
        expectedLog.add(new CrawlLogEntry("Starting crawling thumbnails..."));
        expectedLog.add(new CrawlLogEntry("Crawler step name and its attributes", "Crawler step input data", "Crawler step output data"));
        when(showCrawlerLogStorage.getLastCrawlingLogOfTheShow(showId)).thenReturn(CompletableFuture.completedFuture(Optional.of(expectedLog)));
        // when
        Optional<List<CrawlLogEntry>> possibleLog = showsCrawler.getLastCrawlingLogOfShow(showId);
        // then
        assertEquals(expectedLog, possibleLog.orElse(null));
    }

    @Test
    public void shouldNotGetLastCrawlingLogOfAShowThatHasNotBeenCrawledYet() {
        // given
        when(showCrawlerLogStorage.getLastCrawlingLogOfTheShow(showId)).thenReturn(CompletableFuture.completedFuture(Optional.empty()));
        // when
        Optional<List<CrawlLogEntry>> possibleLog = showsCrawler.getLastCrawlingLogOfShow(showId);
        // then
        assertFalse(possibleLog.isPresent());
    }

    @Test
    public void shouldDeleteCrawlerOfTheShow() {
        // when
        showsCrawler.deleteCrawlerOfShow(showId);
        // then
        verify(showCrawlerStorage).deleteShowCrawlerByShowId(showId);
    }
}
