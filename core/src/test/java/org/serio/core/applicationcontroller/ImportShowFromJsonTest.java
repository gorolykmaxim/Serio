package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.shows.WatchableShow;
import org.serio.core.userinterface.ViewIds;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.when;

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
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldNotImportShowCrawlerWithShowNameThatIsAlreadyTaken() {
        // given
        String showName = setUpShowNameConflict();
        // when
        applicationController.importShowCrawler(rawShowCrawler);
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_OVERRIDE_DIALOG, event.getViewId());
        assertEquals(showName, event.getShowName());
    }

    @Test
    public void shouldImportShowCrawlerWithATakenShowNameAfterUsersOverrideConfirmation() {
        // given
        setUpShowNameConflict();
        applicationController.importShowCrawler(rawShowCrawler);
        reset(userInterface);
        // when
        applicationController.confirmShowOverride();
        applicationController.confirmShowOverride();
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldFailToImportShowCrawler() {
        // given
        when(showsCrawler.crawlShowAndSaveCrawler(rawShowCrawler)).thenThrow(expectedException);
        // when
        applicationController.importShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler(rawShowCrawler);
        // then
        assertCrawlingErrorReceived();
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.ALL_SHOWS);
    }

    private String setUpShowNameConflict() {
        WatchableShow show = shows.findShowById(friends);
        when(showsCrawler.getShowNameDefinedInShowCrawler(rawShowCrawler)).thenReturn(show.getName());
        when(shows.doesShowWithNameExists(show.getName())).thenReturn(true);
        return show.getName();
    }
}
