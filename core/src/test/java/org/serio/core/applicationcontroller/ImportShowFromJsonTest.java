package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.shows.WatchableShow;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.reset;

public class ImportShowFromJsonTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        applicationController.importShowFromJson();
        reset(userInterface);
    }

    @Override
    public void shouldImportShowCrawler() {
        // when
        applicationController.importShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler(rawShowCrawler);
        // then
        List<ApplicationEvent> events = captureUserInterfaceEvents();
        assertEquals(ViewIds.CRAWLING_IN_PROGRESS, events.get(0).getViewId());
        assertEquals(ViewIds.SHOW_DETAILS, events.get(1).getViewId());
        WatchableShow expected = shows.findShowById(friends);
        DisplayableShow actual = ((ShowDetailsEvent)events.get(1)).getShow();
        assertShowEquals(expected, actual, "2 days ago");
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        ApplicationEvent event = captureLastUserInterfaceEvent(ApplicationEvent.class);
        assertEquals(ViewIds.ALL_SHOWS, event.getViewId());
    }
}
