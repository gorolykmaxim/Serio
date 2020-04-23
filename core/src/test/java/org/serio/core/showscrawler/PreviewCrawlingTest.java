package org.serio.core.showscrawler;

import org.apache.commons.lang3.StringUtils;
import org.junit.Before;
import org.junit.Test;
import org.serio.core.showscrawler.tasks.CrawlerTaskException;

import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.when;

public class PreviewCrawlingTest extends BaseShowsCrawlerTest {
    private List<String> expectedOutput;

    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        expectedOutput = IntStream
                .range(1, 11)
                .mapToObj(i -> String.format("%s/episode-%d.mp4", SHOW_URL, i))
                .collect(Collectors.toList());
    }

    @Test
    public void shouldExecuteCrawlerAndReturnItsCrawlingOutput() {
        // when
        CrawlingResult result = showsCrawler.previewCrawler(COMPLETE_VIDEO_CRAWLER);
        // then
        assertEquals(expectedOutput, result.getOutput());
    }

    @Test
    public void shouldExecuteCrawlerAndReturnItsCrawlingLog() {
        // when
        CrawlingResult result = showsCrawler.previewCrawler(COMPLETE_VIDEO_CRAWLER);
        // then
        String log = result.getLog().toString();
        assertTrue(log.contains(StringUtils.truncate(SHOW_WEB_PAGE, (int) crawlerLogDetailsLength / 2)));
    }

    @Test
    public void shouldExecuteCrawlerWithRegExpWithoutMatchingGroups() {
        // given
        String crawler = String.format(COMPLETE_CRAWLER_TEMPLATE, SHOW_URL, "episode-[0-9]+.mp4", "http://tv-shows.com/game-of-thrones/%s");
        // when
        CrawlingResult result = showsCrawler.previewCrawler(crawler);
        // then
        assertEquals(expectedOutput, result.getOutput());
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerWithStepWithoutType() {
        // when
        showsCrawler.previewCrawler(CRAWLER_WITH_STEP_WITHOUT_TYPE);
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerWithStepWithUnknownType() {
        // when
        showsCrawler.previewCrawler(CRAWLER_WITH_STEP_WITH_UNKNOWN_TYPE);
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerWithValueStepWithoutValue() {
        // when
        showsCrawler.previewCrawler(CRAWLER_WITH_VALUE_STEP_WITHOUT_VALUE);
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerWithRegExpStepWithoutRegExp() {
        // when
        showsCrawler.previewCrawler(CRAWLER_WITH_REGEXP_STEP_WITHOUT_REGEXP);
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerWithTransformStepWithoutTemplate() {
        // when
        showsCrawler.previewCrawler(CRAWLER_WITH_TRANSFORM_STEP_WITHOUT_TEMPLATE);
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerDueToHttpClientIOError() {
        // given
        when(httpClient.fetchContentFromUrl(SHOW_URL)).thenReturn(CompletableFuture.supplyAsync(() -> {throw new RuntimeException();}));
        // when
        showsCrawler.previewCrawler(COMPLETE_VIDEO_CRAWLER);
    }

    @Test(expected = CrawlerTaskException.class)
    public void shouldFailToExecuteCrawlerDueToInvalidRegExp() {
        // given
        String crawler = String.format(COMPLETE_NAME_CRAWLER_TEMPLATE, "(.*))", "The %s");
        // when
        showsCrawler.previewCrawler(crawler);
    }
}
