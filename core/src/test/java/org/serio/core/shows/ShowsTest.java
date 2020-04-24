package org.serio.core.shows;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowStorage;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;
import org.serio.core.watchhistory.WatchHistory;

import java.time.Duration;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

public class ShowsTest {
    private ShowStorage showStorage;
    private WatchHistory watchHistory;
    private Shows shows;
    private Show show, anotherShow, notWatchedShow, anotherNotWatchedShow;
    private ShowView showView, anotherShowView;
    private Duration fullyWatched, justStarted;
    private List<EpisodeView> episodeViews;

    @Before
    public void setUp() throws Exception {
        fullyWatched = Duration.ofMinutes(15);
        justStarted = Duration.ZERO;
        List<Episode> episodes = IntStream.range(1, 10)
                .mapToObj(i -> new Episode(i, String.format("http://friends.com/episode%d.mp4", i)))
                .collect(Collectors.toList());
        // Episodes 1 and 2 are fully watched and the last 9th episodes is just started.
        episodeViews = episodes.stream()
                .filter(episode -> episode.getId() < 3 || episode.getId() > 8)
                .map(episode -> new EpisodeView(Long.toString(episode.getId()), episode.getId() == 9 ? justStarted : fullyWatched, LocalDateTime.now().minusHours(episodes.size() - episode.getId())))
                .collect(Collectors.toList());
        show = Show.createNew("Friends", "https://friends.com/thumbnail.jpg", episodes);
        String showId = show.getId().toString();
        showView = new ShowView(showId);
        anotherShow = Show.createNew("How i met your mother", Collections.emptyList());
        anotherShowView = new ShowView(anotherShow.getId().toString(), LocalDateTime.now().minusDays(3));
        notWatchedShow = Show.createNew("Clinic", Collections.emptyList());
        anotherNotWatchedShow = Show.createNew("American Family", "https://family.com/thumbnail.jpg", Collections.emptyList());
        showStorage = mock(ShowStorage.class);
        watchHistory = mock(WatchHistory.class);
        when(showStorage.findById(show.getId())).thenReturn(CompletableFuture.completedFuture(show));
        when(showStorage.findById(notWatchedShow.getId())).thenReturn(CompletableFuture.completedFuture(notWatchedShow));
        when(showStorage.findAll()).thenReturn(CompletableFuture.completedFuture(Stream
                .of(show, notWatchedShow, anotherShow, anotherNotWatchedShow)
                .map(Show::getMetaData)
                .collect(Collectors.toList())
        ));
        when(watchHistory.getShowWatchHistory()).thenReturn(CompletableFuture.completedFuture(Arrays.asList(showView, anotherShowView)));
        when(watchHistory.getEpisodeWatchHistoryOfShow(notWatchedShow.getId().toString())).thenReturn(CompletableFuture.completedFuture(Collections.emptyList()));
        when(watchHistory.getEpisodeWatchHistoryOfShow(showId)).thenReturn(CompletableFuture.completedFuture(episodeViews));
        shows = new Shows(showStorage, watchHistory);
    }

    @Test(expected = AllShowsLookupException.class)
    public void shouldFailToFindAllShowsDueToSomeStorageException() {
        // given
        when(showStorage.findAll()).thenThrow(new RuntimeException());
        // when
        shows.findAllShows();
    }

    @Test
    public void shouldFindMetaDataInformationAboutAllShows() {
        // given
        List<Show> expectedShows = Arrays.asList(show, notWatchedShow, anotherShow, anotherNotWatchedShow);
        // when
        List<WatchableShowMetaData> allShows = shows.findAllShows().getAllShows();
        // then
        for (int i = 0; i < expectedShows.size(); i++) {
            WatchableShowMetaData watchableShow = allShows.get(i);
            Show show = expectedShows.get(i);
            assertEquals(show.getId(), watchableShow.getId());
            assertEquals(show.getName(), watchableShow.getName());
            assertEquals(show.getThumbnailUrl(), watchableShow.getThumbnailUrl());
        }
    }

    @Test
    public void shouldFindAllShowsSomeOfWhichWereWatched() {
        // when
        List<WatchableShowMetaData> allShows = shows.findAllShows().getAllShows();
        // then
        for (WatchableShowMetaData watchableShow: allShows) {
            if (watchableShow.getId().equals(show.getId())) {
                assertEquals(LocalDate.now(), watchableShow.getLastWatchedDate().map(LocalDateTime::toLocalDate).orElse(null));
                assertTrue(watchableShow.hasBeenWatched());
            } else if (watchableShow.getId().equals(anotherShow.getId())) {
                assertEquals(LocalDate.now().minusDays(3), watchableShow.getLastWatchedDate().map(LocalDateTime::toLocalDate).orElse(null));
                assertTrue(watchableShow.hasBeenWatched());
            } else {
                assertFalse(watchableShow.getLastWatchedDate().isPresent());
                assertFalse(watchableShow.hasBeenWatched());
            }
        }
    }

    @Test
    public void shouldFindShowsWatchedLatelyInTheirChronologicalWatchOrderWhereTheRecentlyWatchedShowGoesFirst() {
        // when
        List<WatchableShowMetaData> lastWatchedShows = shows.findAllShows().getLastWatchedShows();
        // then
        assertEquals(2, lastWatchedShows.size());
        assertEquals(show.getId(), lastWatchedShows.get(0).getId());
        assertEquals(anotherShow.getId(), lastWatchedShows.get(1).getId());
    }

    @Test(expected = ShowLookupException.class)
    public void shouldFailToFindSpecifiedShow() {
        // given
        when(showStorage.findById(show.getId())).thenThrow(new RuntimeException());
        // when
        shows.findShowById(show.getId());
    }

    @Test
    public void shouldFindSpecifiedShowWithItsEpisodes() {
        // when
        WatchableShow watchableShow = shows.findShowById(show.getId());
        // then
        assertEquals(show.getId(), watchableShow.getId());
        assertEquals(show.getName(), watchableShow.getName());
        assertEquals(show.getThumbnailUrl(), watchableShow.getThumbnailUrl());
        for (int i = 0; i < show.getEpisodes().size(); i++) {
            Episode episode = show.getEpisodes().get(i);
            WatchableEpisode watchableEpisode = watchableShow.getEpisodes().get(i);
            assertEquals(episode.getId(), watchableEpisode.getId());
            assertEquals(episode.getName(), watchableEpisode.getName());
            assertEquals(episode.getVideoUrl(), watchableEpisode.getVideoUrl());
        }
    }

    @Test
    public void shouldFindSpecifiedShowWhichHasSomeOfItsEpisodesWatched() {
        // when
        WatchableShow watchableShow = shows.findShowById(show.getId());
        // then
        assertTrue(watchableShow.hasBeenWatched());
        assertEquals(showView.getLastWatchedDate(), watchableShow.getLastWatchedDate().orElse(null));
        WatchableEpisode lastWatchedEpisode = watchableShow.getEpisodes().get(8);
        assertEquals(lastWatchedEpisode, watchableShow.getLastWatchedEpisode().orElse(null));
        for (int i = 0; i < show.getEpisodes().size(); i++) {
            Episode episode = show.getEpisodes().get(i);
            WatchableEpisode watchableEpisode = watchableShow.getEpisodes().get(i);
            if (episode.getId() < 3) {
                EpisodeView view = episodeViews.get((int) episode.getId() - 1);
                assertTrue(watchableEpisode.hasBeenWatched());
                assertEquals(view.getLastWatchDate(), watchableEpisode.getLastWatchDate().orElse(null));
                assertEquals(fullyWatched, watchableEpisode.getWatchProgress());
            } else if (episode.getId() == 9) {
                EpisodeView view = episodeViews.get(2);
                assertTrue(watchableEpisode.hasBeenWatched());
                assertEquals(view.getLastWatchDate(), watchableEpisode.getLastWatchDate().orElse(null));
                assertEquals(justStarted, watchableEpisode.getWatchProgress());
            } else {
                assertFalse(watchableEpisode.hasBeenWatched());
                assertFalse(watchableEpisode.getLastWatchDate().isPresent());
                assertEquals(Duration.ZERO, watchableEpisode.getWatchProgress());
            }
        }
    }

    @Test
    public void shouldFindSpecifiedShowWhichHasNotBeenWatched() {
        // when
        WatchableShow watchableShow = shows.findShowById(notWatchedShow.getId());
        // then
        assertFalse(watchableShow.hasBeenWatched());
        assertFalse(watchableShow.getLastWatchedDate().isPresent());
        assertFalse(watchableShow.getLastWatchedEpisode().isPresent());
        for (WatchableEpisode episode: watchableShow.getEpisodes()) {
            assertFalse(episode.getLastWatchDate().isPresent());
            assertEquals(Duration.ZERO, episode.getWatchProgress());
        }
    }

    @Test
    public void shouldSaveSpecifiedShow() {
        // when
        shows.saveShow(show);
        // then
        verify(showStorage).save(show);
    }

    @Test
    public void shouldCreateViewOfTheSpecifiedShowOfTheSpecifiedEpisode() {
        // given
        Episode episode = show.getEpisodes().get(0);
        Duration watchProgress = Duration.ofSeconds(326);
        // when
        shows.watchShowEpisode(show.getId(), episode.getId(), watchProgress);
        // then
        verify(watchHistory).watchShowEpisode(show.getId().toString(), Long.toString(episode.getId()), watchProgress);
    }

    @Test
    public void shouldClearWatchHistoryOfTheSpecifiedShow() {
        // when
        shows.clearWatchHistoryOfShow(show.getId());
        // then
        verify(watchHistory).clearWatchHistoryOfShow(show.getId().toString());
    }

    @Test
    public void shouldDeleteSpecifiedShow() {
        // when
        shows.deleteShow(show.getId());
        // then
        verify(showStorage).deleteById(show.getId());
    }
}
