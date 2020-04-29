package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.CrawlPreviewEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.when;

public class EditCrawlerTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        applicationController.addShow();
        applicationController.editEpisodeVideoCrawler();
        reset(userInterface);
    }

    @Override
    public void shouldSaveCrawler() {
        // given
        applicationController.back();
        applicationController.editEpisodeVideoCrawler();
        // when
        shouldSaveSpecifiedCrawler(CrawlerTypes.EPISODE_VIDEO);
    }

    @Test
    public void shouldSaveThumbnailCrawler() {
        // given
        applicationController.back();
        applicationController.editThumbnailCrawler();
        // when
        shouldSaveSpecifiedCrawler(CrawlerTypes.THUMBNAIL);
    }

    @Test
    public void shouldSaveEpisodeNameCrawler() {
        // given
        applicationController.back();
        applicationController.editEpisodeNameCrawler();
        // when
        shouldSaveSpecifiedCrawler(CrawlerTypes.EPISODE_NAME);
    }

    public void shouldSaveSpecifiedCrawler(String crawlerType) {
        // given
        reset(userInterface);
        // when
        applicationController.saveCrawler(rawCrawler);
        applicationController.saveCrawler(rawCrawler);
        // then
        EditShowCrawlerEvent event = captureLastUserInterfaceEvent(EditShowCrawlerEvent.class);
        assertEquals(rawCrawler, event.getCrawler(crawlerType).orElse(null));
    }

    @Override
    public void shouldPreviewCrawler() {
        // given
        applicationController.back();
        applicationController.editThumbnailCrawler();
        // when
        shouldPreviewSpecifiedCrawler(CrawlerTypes.THUMBNAIL);
    }

    @Test
    public void shouldPreviewEpisodeVideoCrawler() {
        // given
        applicationController.back();
        applicationController.editEpisodeVideoCrawler();
        // when
        shouldPreviewSpecifiedCrawler(CrawlerTypes.EPISODE_VIDEO);
    }

    @Test
    public void shouldPreviewEpisodeNameCrawler() {
        // given
        applicationController.back();
        applicationController.editEpisodeNameCrawler();
        // when
        shouldPreviewSpecifiedCrawler(CrawlerTypes.EPISODE_NAME);
    }

    private void shouldPreviewSpecifiedCrawler(String crawlerType) {
        // given
        reset(userInterface);
        // when
        applicationController.previewCrawler(rawCrawler);
        // then
        List<ApplicationEvent> events = captureUserInterfaceEvents();
        assertEquals(ViewIds.CRAWLING_IN_PROGRESS, events.get(0).getViewId());
        CrawlPreviewEvent event = (CrawlPreviewEvent) events.get(1);
        assertEquals(ViewIds.CRAWL_PREVIEW, event.getViewId());
        assertEquals(crawlerType, event.getCrawlerType());
        assertEquals(previewResults.getOutput(), event.getCrawlItems());
    }

    @Test
    public void shouldFailToPreviewCrawler() {
        // given
        when(showsCrawler.previewCrawler(rawCrawler)).thenThrow(expectedException);
        // when
        applicationController.previewCrawler(rawCrawler);
        applicationController.previewCrawler(rawCrawler);
        // then
        assertCrawlingErrorReceived();
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.EDIT_SHOW_CRAWLER);
    }
}
