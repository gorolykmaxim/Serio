package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.CrawlLogEvent;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.userinterface.ViewIds;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.reset;

public class CrawlPreviewTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        applicationController.addShow();
        applicationController.editThumbnailCrawler();
        applicationController.previewCrawler(rawCrawler);
        reset(userInterface);
    }

    @Override
    public void shouldViewCrawlLog() {
        // given
        applicationController.back();
        applicationController.back();
        applicationController.editThumbnailCrawler();
        applicationController.previewCrawler(rawCrawler);
        // when
        shouldViewCrawlLogOfSpecifiedCrawler(CrawlerTypes.THUMBNAIL);
    }

    @Test
    public void shouldViewCrawlLogOfEpisodeVideoCrawler() {
        // given
        applicationController.back();
        applicationController.back();
        applicationController.editEpisodeVideoCrawler();
        applicationController.previewCrawler(rawCrawler);
        // when
        shouldViewCrawlLogOfSpecifiedCrawler(CrawlerTypes.EPISODE_VIDEO);
    }

    @Test
    public void shouldViewCrawlLogOfEpisodeNameCrawler() {
        // given
        applicationController.back();
        applicationController.back();
        applicationController.editEpisodeNameCrawler();
        applicationController.previewCrawler(rawCrawler);
        // when
        shouldViewCrawlLogOfSpecifiedCrawler(CrawlerTypes.EPISODE_NAME);
    }

    public void shouldViewCrawlLogOfSpecifiedCrawler(String crawlerType) {
        // given
        reset(userInterface);
        // when
        applicationController.viewCrawlLog();
        applicationController.viewCrawlLog();
        // then
        CrawlLogEvent event = captureLastUserInterfaceEvent(CrawlLogEvent.class);
        assertEquals(ViewIds.CRAWL_LOG, event.getViewId());
        assertEquals(crawlerType, event.getCrawlerType().orElse(null));
        assertCrawlLogsEqual(previewResults.getLog(), event.getLogEntries());
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.EDIT_CRAWLER);
    }
}