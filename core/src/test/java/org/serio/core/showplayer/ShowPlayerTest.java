package org.serio.core.showplayer;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.shows.Shows;
import org.serio.core.shows.WatchableShow;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.WatchProgress;

import java.time.LocalDateTime;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.junit.Assert.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

public class ShowPlayerTest {
    private Shows shows;
    private ShowPlayer showPlayer;

    @Before
    public void setUp() throws Exception {
        shows = mock(Shows.class);
        showPlayer = new ShowPlayer(shows);
    }

    @Test(expected = PlayShowException.class)
    public void shouldFailToPlayTheSpecifiedShowDueToSomeException() {
        // given
        when(shows.findShowById(any())).thenThrow(new RuntimeException());
        // when
        showPlayer.playShow(UUID.randomUUID(), false);
    }

    @Test
    public void shouldPlaySpecifiedShowFromTheLastWatchedEpisode() {
        // given
        UUID showId = createShow(10, 5, false);
        // when
        Player player = showPlayer.playShow(showId, false);
        // then
        assertStateOf(player, true, true);
        assertEquals(5, player.getPlayingEpisodeOrFail().getId());
    }

    @Test
    public void shouldPlaySpecifiedShowFromTheEpisodeNextToTheLastCompletelyWatchedEpisode() {
        // given
        UUID showId = createShow(10, 5, true);
        // when
        Player player = showPlayer.playShow(showId, false);
        // then
        assertStateOf(player, true, true);
        assertEquals(6, player.getPlayingEpisodeIdOrFail());
    }

    @Test
    public void shouldPlayTheFirstEpisodeOfUnwatchedShow() {
        // given
        UUID showId = createShow(10, 0, false);
        // when
        Player player = showPlayer.playShow(showId, false);
        // then
        assertStateOf(player, false, true);
        assertEquals(1, player.getPlayingEpisodeIdOrFail());
    }

    @Test
    public void shouldNotPlayAnythingSinceTheLastEpisodeOfTheShowHasBeenWatched() {
        // given
        UUID showId = createShow(10, 10, true);
        // when
        Player player = showPlayer.playShow(showId, false);
        // then
        assertStateOf(player, false, false, false);
    }

    @Test
    public void shouldNotPlayAnythingSinceTheShowDoesNotHaveEpisodes() {
        // given
        UUID showId = createShow(0, 0, false);
        // when
        Player player = showPlayer.playShow(showId, false);
        // then
        assertStateOf(player, false, false, false);
    }

    @Test(expected = PlayShowException.class)
    public void shouldFailToPlaySpecifiedShowFromTheBeginningDueToSomeException() {
        // given
        when(shows.findShowById(any())).thenThrow(new RuntimeException());
        // when
        showPlayer.playShow(UUID.randomUUID(), true);
    }

    @Test
    public void shouldPlaySpecifiedShowFromTheBeginning() {
        // given
        UUID showId = createShow(9, 6, false);
        // when
        Player player = showPlayer.playShow(showId, true);
        // then
        assertStateOf(player, false, true);
        assertEquals(1, player.getPlayingEpisodeIdOrFail());
    }

    @Test
    public void shouldNotPlaySpecifiedShowFromTheBeginningSinceTheShowDoesNotHaveEpisodes() {
        // given
        UUID showId = createShow(0, 0, false);
        // when
        Player player = showPlayer.playShow(showId, true);
        // then
        assertStateOf(player, false, false, false);
    }

    @Test(expected = PlayShowException.class)
    public void shouldFailToPlaySpecifiedEpisodeOfTheShowDueToSomeException() {
        // given
        when(shows.findShowById(any())).thenThrow(new RuntimeException());
        // when
        showPlayer.playShowEpisode(UUID.randomUUID(), 5);
    }

    @Test
    public void shouldPlaySpecifiedEpisodeOfTheShow() {
        // given
        UUID showId = createShow(8, 0, false);
        // when
        Player player = showPlayer.playShowEpisode(showId, 8);
        // then
        assertStateOf(player, true, false);
        assertEquals(8, player.getPlayingEpisodeIdOrFail());
    }

    @Test
    public void shouldNotPlaySpecifiedEpisodeOfTheShowSinceTheEpisodeDoesNotExist() {
        // given
        UUID showId = createShow(5, 3, true);
        // when
        Player player = showPlayer.playShowEpisode(showId, 0);
        // then
        assertStateOf(player, false, false, false);
    }

    @Test(expected = ProgressReportException.class)
    public void shouldNotReportPlaybackProgressSinceNothingIsPlaying() {
        // when
        showPlayer.reportPlaybackProgress(15.3);
    }

    @Test(expected = ProgressReportException.class)
    public void shouldNotReportPlaybackProgressSinceThePlayerIsNotPlaying() {
        // given
        UUID showId = createShow(10, 3, true);
        // when
        showPlayer.playShowEpisode(showId, 11);
        showPlayer.reportPlaybackProgress(35.4);
    }

    @Test(expected = ProgressReportException.class)
    public void shouldFailToReportPlaybackProgressDueToSomeException() {
        // given
        UUID showId = createShow(10, 4, false);
        doThrow(new RuntimeException()).when(shows).watchShowEpisode(eq(showId), eq(3), any());
        // when
        showPlayer.playShowEpisode(showId, 3);
        showPlayer.reportPlaybackProgress(56.34);
    }

    @Test
    public void shouldReportPlaybackProgressOfCurrentlyPlayingShowEpisode() {
        // given
        UUID showId = createShow(10, 4, false);
        // when
        showPlayer.playShowEpisode(showId, 3);
        showPlayer.reportPlaybackProgress(35);
        // then
        verify(shows).watchShowEpisode(showId, 3, WatchProgress.fromPercentage(35));
    }

    @Test(expected = EpisodeChangeException.class)
    public void shouldNotPlayNextEpisodeSinceNothingIsPlaying() {
        // when
        showPlayer.playNextEpisode();
    }

    @Test(expected = EpisodeChangeException.class)
    public void shouldNotPlayNextEpisodeSinceThereIsNoCurrentEpisode() {
        // given
        UUID showId = createShow(10, 3, false);
        // when
        showPlayer.playShowEpisode(showId, 0);
        showPlayer.playNextEpisode();
    }

    @Test
    public void shouldPlayNextEpisodeAndStopSinceCurrentEpisodeIsTheLastOne() {
        // given
        UUID showId = createShow(10, 10, false);
        // when
        showPlayer.playShow(showId, false);
        Player player = showPlayer.playNextEpisode();
        // then
        assertStateOf(player, false, false, false);
    }

    @Test
    public void shouldPlayNextEpisode() {
        // given
        UUID showId = createShow(10, 1, false);
        // when
        showPlayer.playShow(showId, false);
        Player player = showPlayer.playNextEpisode();
        // then
        assertStateOf(player, true, true);
        assertEquals(2, player.getPlayingEpisodeIdOrFail());
    }

    @Test(expected = EpisodeChangeException.class)
    public void shouldNotPlayPreviousEpisodeSinceNothingIsPlaying() {
        // when
        showPlayer.playPreviousEpisode();
    }

    @Test(expected = EpisodeChangeException.class)
    public void shouldNotPlayPreviousEpisodeSinceThereIsNoCurrentEpisode() {
        // given
        UUID showId = createShow(10, 10, true);
        // when
        showPlayer.playShow(showId, false);
        showPlayer.playPreviousEpisode();
    }

    @Test
    public void shouldPlayPreviousEpisodeAndStopSinceCurrentEpisodeIsTheFirstOne() {
        // given
        UUID showId = createShow(10, 5, false);
        // when
        showPlayer.playShow(showId, true);
        Player player = showPlayer.playPreviousEpisode();
        // then
        assertStateOf(player, false, false, false);
    }

    @Test
    public void shouldPlayPreviousEpisode() {
        // given
        UUID showId = createShow(10, 5, false);
        // when
        showPlayer.playShow(showId, false);
        Player player = showPlayer.playPreviousEpisode();
        // then
        assertStateOf(player, true, true);
        assertEquals(4, player.getPlayingEpisodeIdOrFail());
    }

    private void assertStateOf(Player player, boolean hasPreviousEpisode, boolean hasNextEpisode) {
        assertStateOf(player, true, hasPreviousEpisode, hasNextEpisode);
    }

    private void assertStateOf(Player player, boolean isPlaying, boolean hasPreviousEpisode, boolean hasNextEpisode) {
        assertEquals(isPlaying, player.isPlaying());
        assertEquals(hasPreviousEpisode, player.hasPreviousEpisode());
        assertEquals(hasNextEpisode, player.hasNextEpisode());
    }

    private UUID createShow(int episodeCount, int episodesWatched, boolean lastEpisodeCompletelyWatched) {
        LocalDateTime now = LocalDateTime.now();
        List<Episode> episodes = IntStream
                .range(1, episodeCount + 1)
                .mapToObj(i -> new Episode(i, "https://show.com/episode-" + i))
                .collect(Collectors.toList());
        List<EpisodeView> episodeViews = IntStream
                .range(1, episodesWatched + 1)
                .mapToObj(i -> new EpisodeView(Integer.toString(i), i == episodesWatched && !lastEpisodeCompletelyWatched ? WatchProgress.fromPercentage(35) : WatchProgress.COMPLETE, now.minusHours(episodesWatched - i)))
                .collect(Collectors.toList());
        Show show = Show.createNew("The show", "https://show.com/thumbnail.jpg", episodes);
        WatchableShow watchableShow = WatchableShow.from(show, episodeViews);
        when(shows.findShowById(watchableShow.getId())).thenReturn(watchableShow);
        return watchableShow.getId();
    }
}
