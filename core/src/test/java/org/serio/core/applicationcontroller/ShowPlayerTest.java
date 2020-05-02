package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.showplayer.Player;
import org.serio.core.shows.WatchableShow;
import org.serio.core.userinterface.ViewIds;

import java.time.LocalDateTime;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.mockito.Mockito.*;

public class ShowPlayerTest extends BaseApplicationControllerTest {
    private Player player;
    private WatchableShow show;

    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        show = shows.findShowById(friends);
        player = setUpShowPlayer(friends, 1);
        applicationController.viewAllShows();
        applicationController.selectShow(friends.toString());
        applicationController.playShowEpisode(1);
        reset(userInterface);
    }

    @Override
    public void shouldPlayNextEpisode() {
        // given
        when(showPlayer.playNextEpisode()).thenReturn(player.nextEpisode());
        // when
        applicationController.playNextEpisode();
        // then
        assertShowEpisodePlaying(friends, 2);
    }

    @Test
    public void shouldFinishPlayingShowOnNextEpisode() {
        // given
        finishWatchingShow();
        // when
        applicationController.playNextEpisode();
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_WATCH_IS_OVER_DIALOG, event.getViewId());
        assertEquals(show.getName(), event.getShowName());
    }

    @Test
    public void shouldGoBackToAllShowsAfterFinishingAShow() {
        // given
        finishWatchingShow();
        applicationController.playNextEpisode();
        reset(userInterface);
        // when
        applicationController.viewAllShows();
        // then
        assertCurrentView(ViewIds.ALL_SHOWS);
    }

    @Test
    public void shouldFailToGoBackToAllShowsAfterFinishingAShow() {
        // given
        when(shows.findAllShows()).thenThrow(expectedException);
        finishWatchingShow();
        applicationController.playNextEpisode();
        reset(userInterface);
        // when
        applicationController.viewAllShows();
        // then
        assertErrorReceived();
    }

    @Test
    public void shouldStartCurrentShowFromScratchAfterFinishingIt() {
        // given
        setUpShowPlayer(friends, true);
        finishWatchingShow();
        applicationController.playNextEpisode();
        reset(userInterface);
        // when
        applicationController.playShowFromTheBeginning();
        // then
        assertShowEpisodePlaying(friends, 1);
    }

    @Override
    public void shouldPlayPreviousEpisode() {
        // given
        player = setUpShowPlayer(friends, 3);
        when(showPlayer.playPreviousEpisode()).thenReturn(player.previousEpisode());
        applicationController.back();
        applicationController.playShowEpisode(3);
        reset(userInterface);
        // when
        applicationController.playPreviousEpisode();
        // then
        assertShowEpisodePlaying(friends, 2);
    }

    @Override
    public void shouldReportShowWatchProgress() {
        // given
        double progress = 35.5;
        // when
        applicationController.reportShowWatchProgress(progress);
        // then
        verify(showPlayer).reportPlaybackProgress(progress);
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldGoBackToTheShowDetailsViewWhichShouldHaveWatchedEpisodesUpdated() {
        // given
        applicationController.back();
        applicationController.back();
        applicationController.selectShow(mandalorian.toString());
        applicationController.playShow();
        WatchableShow show = shows.findShowById(mandalorian);
        mandalorian = createShow(show.getName(), true, 10, 1, LocalDateTime.now());
        reset(userInterface);
        // when
        applicationController.back();
        // then
        ShowDetailsEvent event = captureLastUserInterfaceEvent(ShowDetailsEvent.class);
        assertFalse(event.getShow().getLastWatched().isEmpty());
    }

    @Test
    public void shouldNotFailOnFailedAttemptOfReportingShowWatchProgress() {
        // given
        doThrow(expectedException).when(showPlayer).reportPlaybackProgress(anyDouble());
        // when
        applicationController.reportShowWatchProgress(32);
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.SHOW_DETAILS);
    }

    private void finishWatchingShow() {
        player = setUpShowPlayer(friends, 10);
        when(showPlayer.playNextEpisode()).thenReturn(player.nextEpisode());
        applicationController.back();
        applicationController.playShowEpisode(10);
        reset(userInterface);
    }
}
