package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.*;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.applicationcontroller.model.DisplayableShowMetaData;
import org.serio.core.shows.WatchableShow;
import org.serio.core.userinterface.ViewIds;

import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.Set;
import java.util.UUID;
import java.util.stream.Collectors;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

public class ShowDetailsTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        applicationController.selectShow(clinic.toString());
        reset(userInterface);
    }

    @Override
    public void shouldPlayShow() {
        // given
        setUpShowPlayer(clinic, false);
        // when
        applicationController.playShow();
        applicationController.playShow();
        // then
        assertShowEpisodePlaying(clinic, 3, true);
    }

    @Test
    public void shouldNotPlayShowSinceUserHasWatchedItCompletely() {
        // given
        setUpShowPlayer(friends, false);
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        reset(userInterface);
        // when
        applicationController.playShow();
        applicationController.playShow();
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_WATCH_IS_OVER_DIALOG, event.getViewId());
    }

    @Test
    public void shouldFailToPlayShow() {
        // given
        when(showPlayer.playShow(clinic, false)).thenThrow(expectedException);
        // when
        applicationController.playShow();
        applicationController.playShow();
        // then
        assertErrorReceived();
    }

    @Override
    public void shouldInitiateClearShowWatchHistory() {
        // given
        WatchableShow show = shows.findShowById(clinic);
        // when
        applicationController.initiateClearShowWatchHistory();
        applicationController.initiateClearShowWatchHistory();
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_CLEAR_WATCH_HISTORY_DIALOG, event.getViewId());
        assertEquals(show.getName(), event.getShowName());
    }

    @Override
    public void shouldPlayShowEpisode() {
        // given
        setUpShowPlayer(clinic, 2);
        // when
        applicationController.playShowEpisode(2);
        applicationController.playShowEpisode(2);
        // then
        assertShowEpisodePlaying(clinic, 2, true);
    }

    @Test
    public void shouldFailToPlayShowEpisode() {
        setUpShowPlayer(clinic, 15);
        // when
        applicationController.playShowEpisode(15);
        applicationController.playShowEpisode(15);
        // then
        ErrorDialogEvent event = captureLastUserInterfaceEvent(ErrorDialogEvent.class);
        assertTrue(event.getErrorMessage().contains("does not have an episode with ID"));
    }

    @Override
    public void shouldCrawlShow() {
        // given
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        reset(userInterface);
        // when
        applicationController.crawlShow();
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldFailToCrawlShow() {
        // given
        when(showsCrawler.crawlShow(clinic.toString())).thenThrow(expectedException);
        // when
        applicationController.crawlShow();
        applicationController.crawlShow();
        // then
        assertCrawlingErrorReceived();
    }

    @Override
    public void shouldInitiateShowRemoval() {
        // given
        WatchableShow show = shows.findShowById(clinic);
        // when
        applicationController.initiateShowRemoval();
        applicationController.initiateShowRemoval();
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_DELETE_SHOW_DIALOG, event.getViewId());
        assertEquals(show.getName(), event.getShowName());
    }

    @Override
    public void shouldViewShowCrawlLog() {
        // given
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        reset(userInterface);
        // when
        applicationController.viewShowCrawlLog();
        applicationController.viewShowCrawlLog();
        // then
        CrawlLogEvent event = captureLastUserInterfaceEvent(CrawlLogEvent.class);
        assertEquals(ViewIds.CRAWL_LOG, event.getViewId());
        assertFalse(event.getCrawlerType().isPresent());
        assertCrawlLogsEqual(previewResults.getLog(), event.getLogEntries());
    }

    @Test
    public void shouldFailToViewShowCrawlLog() {
        // given
        when(showsCrawler.getLastCrawlingLogOfShow(clinic.toString())).thenThrow(expectedException);
        // when
        applicationController.viewShowCrawlLog();
        applicationController.viewShowCrawlLog();
        // then
        assertErrorReceived();
    }

    @Override
    public void shouldEditShowCrawler() {
        // given
        WatchableShow show = shows.findShowById(friends);
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        reset(userInterface);
        // when
        applicationController.editShowCrawler();
        applicationController.editShowCrawler();
        // then
        EditShowCrawlerEvent event = captureLastUserInterfaceEvent(EditShowCrawlerEvent.class);
        assertEquals(ViewIds.EDIT_SHOW_CRAWLER, event.getViewId());
        assertEquals(friends, event.getShowId().orElse(null));
        assertEquals(show.getName(), event.getShowName().orElse(null));
        Arrays.asList(CrawlerTypes.THUMBNAIL, CrawlerTypes.EPISODE_NAME, CrawlerTypes.EPISODE_VIDEO)
                .forEach(type -> assertEquals(rawCrawler, event.getCrawler(type).orElse(null)));
    }

    @Test
    public void shouldFailToEditShowCrawler() {
        // given
        when(showsCrawler.getSerializedPartsOfCrawlerOfShow(clinic.toString())).thenThrow(expectedException);
        // when
        applicationController.editShowCrawler();
        applicationController.editShowCrawler();
        // then
        assertErrorReceived();
    }

    @Test
    public void shouldNotClearWatchHistory() {
        // given
        applicationController.initiateClearShowWatchHistory();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.SHOW_DETAILS);
        verify(shows, never()).clearWatchHistoryOfShow(clinic);
    }

    @Override
    public void shouldClearWatchHistory() {
        // given
        applicationController.initiateClearShowWatchHistory();
        reset(userInterface);
        // when
        applicationController.clearWatchHistory();
        applicationController.clearWatchHistory();
        // then
        assertCurrentView(ViewIds.SHOW_DETAILS);
        verify(shows).clearWatchHistoryOfShow(clinic);
    }

    @Test
    public void shouldClearWatchHistoryAndUpdateAllShowsViewAccordingly() {
        // given
        clinic = createShow("Clinic", false, 5, 0, LocalDateTime.now());
        setUpAllShows(friends, clinic, office, mandalorian);
        applicationController.initiateClearShowWatchHistory();
        applicationController.clearWatchHistory();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        AllShowsEvent event = captureLastUserInterfaceEvent(AllShowsEvent.class);
        Set<UUID> watchedShowIds = event
                .getLastWatchedShows()
                .stream()
                .map(DisplayableShowMetaData::getId)
                .collect(Collectors.toSet());
        assertFalse(watchedShowIds.contains(clinic));
    }

    @Test
    public void shouldFailToClearWatchHistory() {
        // given
        doThrow(expectedException).when(shows).clearWatchHistoryOfShow(clinic);
        applicationController.initiateClearShowWatchHistory();
        reset(userInterface);
        // when
        applicationController.clearWatchHistory();
        applicationController.clearWatchHistory();
        // then
        assertErrorReceived();
    }

    @Test
    public void shouldNotDeleteShow() {
        // given
        applicationController.initiateShowRemoval();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.SHOW_DETAILS);
        verify(shows, never()).deleteShow(clinic);
    }

    @Override
    public void shouldDeleteShow() {
        // given
        applicationController.initiateShowRemoval();
        reset(userInterface);
        // when
        applicationController.deleteShow();
        // then
        assertCurrentView(ViewIds.ALL_SHOWS);
        verify(shows).deleteShow(clinic);
    }

    @Test
    public void shouldFailToDeleteShow() {
        // given
        doThrow(expectedException).when(shows).deleteShow(clinic);
        applicationController.initiateShowRemoval();
        reset(userInterface);
        // when
        applicationController.deleteShow();
        applicationController.deleteShow();
        // then
        assertErrorReceived();
    }

    @Override
    public void shouldShareShowCrawler() {
        // given
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        reset(userInterface);
        // when
        applicationController.shareShowCrawler();
        // then
        verify(userInterface, never()).sendEvent(any());
        verify(clipboard).setContent(rawShowCrawler);
        verify(notifications).displayTextAsNotification("Crawler is copied to your clipboard");
    }

    @Test
    public void shouldFailToShareShowCrawler() {
        // given
        when(showsCrawler.getSerializedCrawlerOfShow(clinic.toString())).thenThrow(expectedException);
        // when
        applicationController.shareShowCrawler();
        // then
        assertErrorReceived();
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.ALL_SHOWS);
    }
}
