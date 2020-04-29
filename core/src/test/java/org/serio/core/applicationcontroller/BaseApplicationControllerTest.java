package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.ErrorDialogEvent;
import org.serio.core.applicationcontroller.model.DisplayableEpisode;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.model.DisplayableShowMetaData;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.notifications.Notifications;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.shows.*;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;
import org.serio.core.userinterface.ViewIds;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;
import org.serio.core.watchhistory.WatchProgress;

import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

public abstract class BaseApplicationControllerTest {
    protected Clipboard clipboard;
    protected Notifications notifications;
    protected ShowPlayer showPlayer;
    protected Shows shows;
    protected ShowsCrawler showsCrawler;
    protected UserInterface userInterface;
    protected ApplicationController applicationController;
    protected UUID friends, office, clinic, mandalorian;
    protected String rawCrawler, rawShowCrawler;
    protected Exception expectedException;

    @Before
    public void setUp() throws Exception {
        expectedException = new RuntimeException("Expected exception");
        clipboard = mock(Clipboard.class);
        notifications = mock(Notifications.class);
        showPlayer = mock(ShowPlayer.class);
        shows = mock(Shows.class);
        showsCrawler = mock(ShowsCrawler.class);
        userInterface = mock(UserInterface.class);
        setUpShows();
        setUpShowsCrawler();
        applicationController = new ApplicationController(clipboard, notifications, showPlayer, shows, showsCrawler, userInterface);
    }

    private void setUpShows() {
        LocalDateTime now = LocalDateTime.now();
        friends = createShow("Friends", true, 10, 10, now.minusDays(2));
        clinic = createShow("Clinic", false, 5, 2, now.minusDays(1));
        office = createShow("Office", true, 15, 13, now);
        mandalorian = createShow("Mandalorian", true, 10, 0, now);
        List<WatchableShow> allShows = Stream.of(friends, clinic, office, mandalorian)
                .map(shows::findShowById)
                .collect(Collectors.toList());
        List<ShowMetaData> allShowsMetaData = allShows
                .stream()
                .map(show -> new ShowMetaData(show.getId(), show.getName(), show.getThumbnailUrl()))
                .collect(Collectors.toList());
        List<ShowView> showViews = allShows
                .stream()
                .filter(WatchableShow::hasBeenWatched)
                .map(show -> new ShowView(show.getId().toString(), show.getLastWatchedDate().get()))
                .collect(Collectors.toList());
        WatchableShowList showList = WatchableShowList.from(allShowsMetaData, showViews);
        when(shows.findAllShows()).thenReturn(showList);
    }

    private void setUpShowsCrawler() {
        rawCrawler = "Raw crawler";
        rawShowCrawler = "Raw show crawler";
        WatchableShow watchableShow = shows.findShowById(friends);
        List<Episode> episodes = watchableShow
                .getEpisodes()
                .stream()
                .map(episode -> new Episode(episode.getId(), episode.getVideoUrl()))
                .collect(Collectors.toList());
        Show show = Show.createNew(watchableShow.getName(), watchableShow.getThumbnailUrl(), episodes);
        when(showsCrawler.crawlShowAndSaveCrawler(rawShowCrawler)).thenReturn(show);
    }

    @Test
    public void shouldViewAllShows() {
        // when
        applicationController.viewAllShows();
        // then
        AllShowsEvent event = captureLastUserInterfaceEvent(AllShowsEvent.class);
        assertEquals(ViewIds.ALL_SHOWS, event.getViewId());
        Set<UUID> showIds = event.getAllShows().stream().map(DisplayableShowMetaData::getId).collect(Collectors.toSet());
        assertTrue(showIds.containsAll(Arrays.asList(friends, clinic, office, mandalorian)));
        List<DisplayableShowMetaData> lastWatchedShows = event.getLastWatchedShows();
        WatchableShowList showList = shows.findAllShows();
        List<WatchableShowMetaData> expectedLastWatchedShows = showList.getLastWatchedShows();
        for (int i = 0; i < expectedLastWatchedShows.size(); i++) {
            DisplayableShowMetaData actual = lastWatchedShows.get(i);
            WatchableShowMetaData expected = expectedLastWatchedShows.get(i);
            assertEquals(expected.getId(), actual.getId());
            assertEquals(expected.getName(), actual.getName());
            assertEquals(expected.getThumbnailUrl(), actual.getThumbnail());
            if (actual.getId().equals(office)) {
                assertEquals("Today", actual.getLastWatched());
            } else if (actual.getId().equals(clinic)) {
                assertEquals("Yesterday", actual.getLastWatched());
            } else if (actual.getId().equals(friends)) {
                assertEquals("2 days ago", actual.getLastWatched());
            }
        }
    }

    @Test
    public void shouldFailToViewAllShows() {
        // given
        when(shows.findAllShows()).thenThrow(expectedException);
        // when
        applicationController.viewAllShows();
        // then
        ErrorDialogEvent event = captureLastUserInterfaceEvent(ErrorDialogEvent.class);
        assertEquals(ViewIds.SHOW_ERROR_DIALOG, event.getViewId());
        assertEquals(expectedException.getMessage(), event.getErrorMessage());
    }

    @Test
    public void shouldAddShow() {
        // when
        applicationController.addShow();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldImportShowFromJson() {
        // when
        applicationController.importShowFromJson();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldSelectShow() {
        // when
        applicationController.selectShow("");
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldEditThumbnailCrawler() {
        // when
        applicationController.editThumbnailCrawler();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldEditEpisodeVideoCrawler() {
        // when
        applicationController.editEpisodeVideoCrawler();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldEditEpisodeNameCrawler() {
        // when
        applicationController.editEpisodeNameCrawler();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldSaveCrawler() {
        // when
        applicationController.saveCrawler("");
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldSaveShowCrawler() {
        // when
        applicationController.saveShowCrawler("");
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldImportShowCrawler() {
        // when
        applicationController.importShowCrawler("");
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldPreviewCrawler() {
        // when
        applicationController.previewCrawler("");
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldViewCrawlLog() {
        // when
        applicationController.viewCrawlLog();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldViewLogEntry() {
        // when
        applicationController.viewLogEntry(0);
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldPlayShow() {
        // when
        applicationController.playShow();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldPlayShowEpisode() {
        // when
        applicationController.playShowEpisode(0);
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldEditShowCrawler() {
        // when
        applicationController.editShowCrawler();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldShareShowCrawler() {
        // when
        applicationController.shareShowCrawler();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldCrawlShow() {
        // when
        applicationController.crawlShow();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldViewShowCrawlLog() {
        // when
        applicationController.viewShowCrawlLog();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldInitiateClearShowWatchHistory() {
        // when
        applicationController.initiateClearShowWatchHistory();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldInitiateShowRemoval() {
        // when
        applicationController.initiateShowRemoval();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldReportShowWatchProgress() {
        // when
        applicationController.reportShowWatchProgress(0);
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldPlayPreviousEpisode() {
        // when
        applicationController.playPreviousEpisode();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldPlayNextEpisode() {
        // when
        applicationController.playNextEpisode();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldClearWatchHistory() {
        // when
        applicationController.clearWatchHistory();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldDeleteShow() {
        // when
        applicationController.deleteShow();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public void shouldPlayShowFromTheBeginning() {
        // when
        applicationController.playShowFromTheBeginning();
        // then
        verify(userInterface, never()).sendEvent(any());
    }

    @Test
    public abstract void shouldBack();

    private UUID createShow(String name, boolean hasThumbnail, int episodeCount, int episodesWatched, LocalDateTime lastWatchedDate) {
        List<Episode> episodes = IntStream
                .range(1, episodeCount + 1)
                .mapToObj(i -> new Episode(i, "https://show.com/episode-" + i))
                .collect(Collectors.toList());
        List<EpisodeView> episodeViews = IntStream
                .range(1, episodesWatched + 1)
                .mapToObj(i -> new EpisodeView(Integer.toString(i), WatchProgress.COMPLETE, lastWatchedDate.minusHours(episodesWatched - i)))
                .collect(Collectors.toList());
        Show show = Show.createNew(name, hasThumbnail ? "https://show.com/thumbnail.jpg" : null, episodes);
        WatchableShow watchableShow = WatchableShow.from(show, episodeViews);
        when(shows.findShowById(watchableShow.getId())).thenReturn(watchableShow);
        return watchableShow.getId();
    }

    protected  <T extends ApplicationEvent> T captureLastUserInterfaceEvent(Class<T> expectedClass) {
        ArgumentCaptor<T> captor = ArgumentCaptor.forClass(expectedClass);
        verify(userInterface).sendEvent(captor.capture());
        return captor.getValue();
    }

    protected List<ApplicationEvent> captureUserInterfaceEvents() {
        ArgumentCaptor<ApplicationEvent> captor = ArgumentCaptor.forClass(ApplicationEvent.class);
        verify(userInterface, atLeastOnce()).sendEvent(captor.capture());
        return captor.getAllValues();
    }

    protected void assertShowEquals(WatchableShow expected, DisplayableShow actual, String expectedLastWatched) {
        assertEquals(expected.getId(), actual.getId());
        assertEquals(expected.getName(), actual.getName());
        assertEquals(expected.getThumbnailUrl(), actual.getThumbnail());
        assertEquals(expectedLastWatched, actual.getLastWatched());
        for (int i = 0; i < expected.getEpisodes().size(); i++) {
            WatchableEpisode e = expected.getEpisodes().get(i);
            DisplayableEpisode a = actual.getEpisodes().get(i);
            assertEquals(e.getId(), a.getId());
            assertEquals(e.getName(), a.getName());
            assertEquals(e.getVideoUrl(), a.getVideoUrl());
            assertEquals(e.hasBeenWatched(), a.isWatched());
        }
    }
}
