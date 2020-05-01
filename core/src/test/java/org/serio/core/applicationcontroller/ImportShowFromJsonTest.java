package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DisplayableShowMetaData;
import org.serio.core.shows.WatchableShow;
import org.serio.core.userinterface.ViewIds;

import java.util.Set;
import java.util.UUID;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
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
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
        applicationController.importShowCrawler();
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldAddCrawledShowToTheAllShowsView() {
        // given
        setUpAllShows(friends, clinic, office, mandalorian);
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
        applicationController.importShowCrawler();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        AllShowsEvent event = captureLastUserInterfaceEvent(AllShowsEvent.class);
        Set<UUID> allShowIds = event
                .getAllShows()
                .stream()
                .map(DisplayableShowMetaData::getId)
                .collect(Collectors.toSet());
        assertTrue(allShowIds.contains(friends));
    }

    @Test
    public void shouldFailToAddCrawledShowToTheAllShowsViewButStillDisplayTheCrawledShow() {
        // given
        when(shows.findAllShows()).thenThrow(expectedException);
        // when
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
        applicationController.importShowCrawler();
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldNotImportShowCrawlerWithShowNameThatIsAlreadyTaken() {
        // given
        String showName = setUpShowNameConflict();
        // when
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_OVERRIDE_DIALOG, event.getViewId());
        assertEquals(showName, event.getShowName());
    }

    @Test
    public void shouldImportShowCrawlerWithATakenShowNameAfterUsersOverrideConfirmation() {
        // given
        setUpShowNameConflict();
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
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
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
        applicationController.importShowCrawler();
        // then
        assertCrawlingErrorReceived();
    }

    @Test
    public void shouldFailToImportShowCrawlerAndGoBackToImportViewWithSavedShowCrawler() {
        // given
        when(showsCrawler.crawlShowAndSaveCrawler(rawShowCrawler)).thenThrow(expectedException);
        applicationController.setShowCrawler(rawShowCrawler);
        applicationController.importShowCrawler();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        ImportShowFromJsonEvent event = captureLastUserInterfaceEvent(ImportShowFromJsonEvent.class);
        assertEquals(rawShowCrawler, event.getShowCrawler().orElse(null));
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
