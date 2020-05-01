package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.CrawlLogEntryDetailsEvent;
import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.userinterface.ViewIds;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.reset;

public class CrawlLogTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        applicationController.addShow();
        applicationController.editEpisodeVideoCrawler();
        applicationController.setCrawler(rawCrawler);
        applicationController.previewCrawler();
        applicationController.viewCrawlLog();
        reset(userInterface);
    }

    @Override
    public void shouldViewLogEntry() {
        // given
        int entryId = 0;
        CrawlLogEntry expected = previewResults.getLog().get(entryId);
        // when
        applicationController.viewLogEntry(entryId);
        applicationController.viewLogEntry(entryId);
        // then
        CrawlLogEntryDetailsEvent event = captureLastUserInterfaceEvent(CrawlLogEntryDetailsEvent.class);
        assertEquals(ViewIds.CRAWL_LOG_ENTRY_DETAILS, event.getViewId());
        IndexedCrawlLogEntry actual = event.getLogEntry();
        assertEquals(entryId, actual.getId());
        assertEquals(expected.hasDetails(), actual.getHasDetails());
        assertEquals(expected.getValue(), actual.getDescription());
        assertEquals(expected.getInputInformation(), actual.getInput());
        assertEquals(expected.getOutputInformation(), actual.getOutput());
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.CRAWL_PREVIEW);
    }

    @Test
    public void shouldBackToShowDetails() {
        // given
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        applicationController.viewShowCrawlLog();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.SHOW_DETAILS);
    }
}
