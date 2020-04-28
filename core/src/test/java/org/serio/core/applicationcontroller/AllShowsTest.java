package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.ErrorDialogEvent;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.userinterface.ViewIds;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

public class AllShowsTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        reset(userInterface);
    }

    @Override
    public void shouldAddShow() {
        // when
        applicationController.addShow();
        applicationController.addShow();
        // then
        EditShowCrawlerEvent event = captureLastUserInterfaceEvent(EditShowCrawlerEvent.class);
        assertEquals(ViewIds.EDIT_SHOW_CRAWLER, event.getViewId());
        assertFalse(event.getCrawler(CrawlerTypes.THUMBNAIL).isPresent());
        assertFalse(event.getCrawler(CrawlerTypes.EPISODE_VIDEO).isPresent());
        assertFalse(event.getCrawler(CrawlerTypes.EPISODE_NAME).isPresent());
        assertFalse(event.getShowId().isPresent());
        assertFalse(event.getShowName().isPresent());
    }

    @Override
    public void shouldImportShowFromJson() {
        // when
        applicationController.importShowFromJson();
        applicationController.importShowFromJson();
        // then
        ImportShowFromJsonEvent event = captureLastUserInterfaceEvent(ImportShowFromJsonEvent.class);
        assertEquals(ViewIds.IMPORT_SHOW_CRAWLER, event.getViewId());
    }

    @Override
    public void shouldSelectShow() {
        // when
        applicationController.selectShow(friends.toString());
        applicationController.selectShow(clinic.toString());
        // then
        ShowDetailsEvent event = captureLastUserInterfaceEvent(ShowDetailsEvent.class);
        assertEquals(ViewIds.SHOW_DETAILS, event.getViewId());
        assertShowEquals(shows.findShowById(friends), event.getShow(), "2 days ago");
    }

    @Test
    public void shouldFailToSelectShow() {
        // given
        when(shows.findShowById(friends)).thenThrow(expectedException);
        // when
        applicationController.selectShow(friends.toString());
        applicationController.selectShow(friends.toString());
        // then
        ErrorDialogEvent event = captureLastUserInterfaceEvent(ErrorDialogEvent.class);
        assertEquals(ViewIds.SHOW_ERROR_DIALOG, event.getViewId());
        assertEquals(expectedException.getMessage(), event.getErrorMessage());
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        verify(userInterface, never()).sendEvent(any());
    }
}
