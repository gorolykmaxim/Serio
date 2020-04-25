package org.serio.core.shows;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.showstorage.ShowStorage;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;
import org.serio.core.watchhistory.WatchHistory;
import org.serio.core.watchhistory.WatchProgress;

import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

public class ShowsTest {
    private ShowStorage showStorage;
    private WatchHistory watchHistory;
    private Shows shows;
    private UUID watchedShow1, watchedShow2, showWithoutEpisodes, notWatchedShow;
    private LocalDateTime watchedShow1LastWatchDate, watchedShow2LastWatchDate;

    @Before
    public void setUp() throws Exception {
        showStorage = mock(ShowStorage.class);
        watchHistory = mock(WatchHistory.class);
        LocalDateTime now = LocalDateTime.now();
        watchedShow1LastWatchDate = now;
        watchedShow2LastWatchDate = now.minusDays(2);
        watchedShow1 = createShow("Show 1", "https://show-1.com/thumbnail.jpg", 10, watchedShow1LastWatchDate);
        watchedShow2 = createShow("Show 2", null, 6, watchedShow2LastWatchDate);
        showWithoutEpisodes = createShow("Show 3", null, 0, null);
        notWatchedShow = createShow("Show 4", null, 5, null);
        ShowView watchedShow1View = new ShowView(watchedShow1.toString(), watchedShow1LastWatchDate);
        ShowView watchedShow2View = new ShowView(watchedShow2.toString(), watchedShow2LastWatchDate);
        when(watchHistory.getShowWatchHistory()).thenReturn(CompletableFuture.completedFuture(Arrays.asList(watchedShow1View, watchedShow2View)));
        List<ShowMetaData> allShows = Stream.of(watchedShow1, watchedShow2, showWithoutEpisodes, notWatchedShow)
                .map(showStorage::findById)
                .map(future -> {
                    try {
                        return future.get();
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                })
                .map(Show::getMetaData)
                .collect(Collectors.toList());
        when(showStorage.findAll()).thenReturn(CompletableFuture.completedFuture(allShows));
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
    public void shouldFindMetaDataInformationAboutAllShows() throws ExecutionException, InterruptedException {
        // given
        List<ShowMetaData> expectedShows = showStorage.findAll().get();
        // when
        List<WatchableShowMetaData> allShows = shows.findAllShows().getAllShows();
        // then
        for (int i = 0; i < expectedShows.size(); i++) {
            WatchableShowMetaData watchableShow = allShows.get(i);
            ShowMetaData show = expectedShows.get(i);
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
            if (watchedShow1.equals(watchableShow.getId())) {
                assertEquals(watchedShow1LastWatchDate, watchableShow.getLastWatchedDate().orElse(null));
                assertTrue(watchableShow.hasBeenWatched());
            } else if (watchedShow2.equals(watchableShow.getId())) {
                assertEquals(watchedShow2LastWatchDate, watchableShow.getLastWatchedDate().orElse(null));
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
        assertEquals(watchedShow1, lastWatchedShows.get(0).getId());
        assertEquals(watchedShow2, lastWatchedShows.get(1).getId());
    }

    @Test(expected = ShowLookupException.class)
    public void shouldFailToFindSpecifiedShow() {
        // given
        when(showStorage.findById(watchedShow1)).thenThrow(new RuntimeException());
        // when
        shows.findShowById(watchedShow1);
    }

    @Test
    public void shouldFindSpecifiedShowWithItsEpisodes() throws ExecutionException, InterruptedException {
        // given
        Show show = showStorage.findById(watchedShow1).get();
        List<Episode> episodes = new ArrayList<>(show.getEpisodes());
        episodes.sort(Comparator.comparing(Episode::getId));
        // when
        WatchableShow watchableShow = shows.findShowById(watchedShow1);
        // then
        assertEquals(show.getId(), watchableShow.getId());
        assertEquals(show.getName(), watchableShow.getName());
        assertEquals(show.getThumbnailUrl(), watchableShow.getThumbnailUrl());
        for (int i = 0; i < show.getEpisodes().size(); i++) {
            Episode episode = episodes.get(i);
            WatchableEpisode watchableEpisode = watchableShow.getEpisodes().get(i);
            assertEquals(episode.getId(), watchableEpisode.getId());
            assertEquals(episode.getName(), watchableEpisode.getName());
            assertEquals(episode.getVideoUrl(), watchableEpisode.getVideoUrl());
        }
    }

    @Test
    public void shouldFindSpecifiedShowWhichHasSomeOfItsEpisodesWatched() {
        // when
        WatchableShow watchableShow = shows.findShowById(watchedShow1);
        // then
        assertTrue(watchableShow.hasBeenWatched());
        assertEquals(watchedShow1LastWatchDate, watchableShow.getLastWatchedDate().orElse(null));
        assertEquals(watchedShow1LastWatchDate, watchableShow.getLastWatchedEpisode().flatMap(WatchableEpisode::getLastWatchDate).orElse(null));
        int episodeCount = watchableShow.getEpisodes().size();
        for (int i = 0; i < episodeCount; i++) {
            WatchableEpisode episode = watchableShow.getEpisodes().get(i);
            if (episode.getId() > episodeCount / 2) {
                assertTrue(episode.hasBeenWatched());
                assertEquals(watchedShow1LastWatchDate.minusHours(episodeCount - episode.getId()), episode.getLastWatchDate().orElse(null));
                if (episode.getId() == episodeCount) {
                    assertFalse(episode.hasBeenWatchedCompletely());
                    assertEquals(WatchProgress.NONE, episode.getWatchProgress());
                } else {
                    assertTrue(episode.hasBeenWatchedCompletely());
                    assertEquals(WatchProgress.COMPLETE, episode.getWatchProgress());
                }
            } else {
                assertFalse(episode.hasBeenWatched());
                assertFalse(episode.hasBeenWatchedCompletely());
                assertFalse(episode.getLastWatchDate().isPresent());
                assertEquals(WatchProgress.NONE, episode.getWatchProgress());
            }
        }
    }

    @Test
    public void shouldFindSpecifiedShowWhichHasNotBeenWatched() {
        // when
        WatchableShow watchableShow = shows.findShowById(notWatchedShow);
        // then
        assertFalse(watchableShow.hasBeenWatched());
        assertFalse(watchableShow.getLastWatchedDate().isPresent());
        assertFalse(watchableShow.getLastWatchedEpisode().isPresent());
        for (WatchableEpisode episode: watchableShow.getEpisodes()) {
            assertFalse(episode.getLastWatchDate().isPresent());
            assertEquals(WatchProgress.NONE, episode.getWatchProgress());
        }
    }

    @Test
    public void shouldGetEpisodeById() {
        // then
        assertTrue(shows.findShowById(watchedShow1).getEpisodeById(3).isPresent());
    }

    @Test
    public void shouldNotGetEpisodeById() {
        // then
        assertFalse(shows.findShowById(watchedShow1).getEpisodeById(0).isPresent());
        assertFalse(shows.findShowById(showWithoutEpisodes).getEpisodeById(1).isPresent());
    }

    @Test
    public void shouldGetFirstEpisodeOfTheShow() {
        // then
        assertTrue(shows.findShowById(watchedShow1).getFirstEpisode().isPresent());
    }

    @Test
    public void shouldNotGetFirstEpisodeOfAnEmptyShow() {
        // then
        assertFalse(shows.findShowById(showWithoutEpisodes).getFirstEpisode().isPresent());
    }

    @Test
    public void shouldGetEpisodeBeforeEpisode() {
        // when
        WatchableEpisode previousEpisode = shows.findShowById(watchedShow1).getEpisodeBeforeEpisode(2).orElse(null);
        // then
        assertEquals(1, previousEpisode.getId());
    }

    @Test
    public void shouldNotGetEpisodeBeforeEpisode() {
        // then
        assertFalse(shows.findShowById(watchedShow1).getEpisodeBeforeEpisode(1).isPresent());
        assertFalse(shows.findShowById(watchedShow1).getEpisodeBeforeEpisode(999).isPresent());
    }

    @Test
    public void shouldGetEpisodeAfterEpisode() {
        // when
        WatchableEpisode nextEpisode = shows.findShowById(watchedShow1).getEpisodeAfterEpisode(9).orElse(null);
        // then
        assertEquals(10, nextEpisode.getId());
    }

    @Test
    public void shouldNotGetEpisodeAfterEpisode() {
        // then
        assertFalse(shows.findShowById(watchedShow1).getEpisodeAfterEpisode(10).isPresent());
    }

    @Test
    public void shouldSaveSpecifiedShow() throws ExecutionException, InterruptedException {
        // given
        Show show = showStorage.findById(watchedShow1).get();
        // when
        shows.saveShow(show);
        // then
        verify(showStorage).save(show);
    }

    @Test
    public void shouldCreateViewOfTheSpecifiedShowOfTheSpecifiedEpisode() throws ExecutionException, InterruptedException {
        // given
        Show show = showStorage.findById(watchedShow1).get();
        Episode episode = show.getEpisodes().get(0);
        WatchProgress watchProgress = WatchProgress.fromPercentage(15);
        // when
        shows.watchShowEpisode(show.getId(), episode.getId(), watchProgress);
        // then
        verify(watchHistory).watchShowEpisode(show.getId().toString(), Integer.toString(episode.getId()), watchProgress);
    }

    @Test
    public void shouldClearWatchHistoryOfTheSpecifiedShow() {
        // when
        shows.clearWatchHistoryOfShow(watchedShow1);
        // then
        verify(watchHistory).clearWatchHistoryOfShow(watchedShow1.toString());
    }

    @Test
    public void shouldDeleteSpecifiedShow() {
        // when
        shows.deleteShow(watchedShow1);
        // then
        verify(showStorage).deleteById(watchedShow1);
    }

    private UUID createShow(String name, String thumbnail, int episodesCount, LocalDateTime lastWatchDate) {
        List<Episode> episodes;
        List<EpisodeView> episodeViews;
        if (episodesCount == 0) {
            episodes = Collections.emptyList();
        } else {
            episodes = IntStream
                    .range(1, episodesCount + 1)
                    .mapToObj(i -> new Episode(i, String.format("http://show.com/episode%d.mp4", i)))
                    .collect(Collectors.toList());
        }
        if (!episodes.isEmpty() && lastWatchDate != null) {
            episodeViews = episodes
                    .stream()
                    .filter(episode -> episode.getId() > episodesCount / 2)
                    .map(episode -> new EpisodeView(Integer.toString(episode.getId()), episode.getId() == episodesCount ? WatchProgress.NONE : WatchProgress.COMPLETE, lastWatchDate.minusHours(episodesCount - episode.getId())))
                    .collect(Collectors.toList());
        } else {
            episodeViews = Collections.emptyList();
        }
        episodes = new ArrayList<>(episodes);
        Collections.shuffle(episodes);
        Show show = Show.createNew(name, thumbnail, episodes);
        when(watchHistory.getEpisodeWatchHistoryOfShow(show.getId().toString())).thenReturn(CompletableFuture.completedFuture(episodeViews));
        when(showStorage.findById(show.getId())).thenReturn(CompletableFuture.completedFuture(show));
        return show.getId();
    }
}
