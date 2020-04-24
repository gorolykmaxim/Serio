package org.serio.core.watchhistory;

import org.junit.Before;
import org.junit.Test;

import java.time.LocalDate;
import java.time.LocalDateTime;

import static org.junit.Assert.*;

public class EpisodeViewTest {
    private String episodeId;
    private WatchProgress watchProgress;
    private LocalDateTime lastWatchedDate;

    @Before
    public void setUp() throws Exception {
        episodeId = "15";
        watchProgress = WatchProgress.fromPercentage(15);
        lastWatchedDate = LocalDateTime.now().minusDays(2);
    }

    @Test
    public void shouldHaveSpecifiedEpisodeId() {
        // when
        EpisodeView view = new EpisodeView(episodeId);
        // then
        assertEquals(episodeId, view.getEpisodeId());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveEpisodeIdNotSpecified() {
        // when
        new EpisodeView(null);
    }

    @Test
    public void shouldHaveSpecifiedWatchProgress() {
        // when
        EpisodeView view = new EpisodeView(episodeId, watchProgress);
        // then
        assertEquals(watchProgress, view.getWatchProgress());
    }

    @Test
    public void shouldHaveZeroWatchProgressIfNotSpecified() {
        // when
        EpisodeView view = new EpisodeView(episodeId);
        // then
        assertEquals(WatchProgress.NONE, view.getWatchProgress());
    }

    @Test
    public void shouldBeCompleteIfTheWatchProgressIsComplete() {
        // when
        EpisodeView view = new EpisodeView(episodeId, WatchProgress.COMPLETE);
        // then
        assertTrue(view.isComplete());
    }

    @Test
    public void shouldNotBeCompleteIfTheWatchProgressIsNotComplete() {
        // when
        EpisodeView view = new EpisodeView(episodeId, WatchProgress.fromPercentage(99));
        // then
        assertFalse(view.isComplete());
    }

    @Test
    public void shouldHaveSpecifiedLastWatchedDate() {
        // when
        EpisodeView view = new EpisodeView(episodeId, watchProgress, lastWatchedDate);
        // then
        assertEquals(lastWatchedDate, view.getLastWatchDate());
    }

    @Test
    public void shouldHaveLastWatchedDateSetToTodayIfNotSpecified() {
        // when
        EpisodeView view = new EpisodeView(episodeId, watchProgress);
        // then
        assertEquals(LocalDate.now(), view.getLastWatchDate().toLocalDate());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // when
        EpisodeView view1 = new EpisodeView(episodeId, watchProgress, lastWatchedDate);
        EpisodeView view2 = new EpisodeView(episodeId, watchProgress, lastWatchedDate);
        // then
        assertEquals(view1, view2);
        assertEquals(view1.hashCode(), view2.hashCode());
    }

    @Test
    public void shouldNotBeEqual() {
        // when
        EpisodeView view1 = new EpisodeView(episodeId);
        EpisodeView view2 = new EpisodeView(episodeId, watchProgress, lastWatchedDate);
        // then
        assertNotEquals(view1, view2);
        assertNotEquals(view1.hashCode(), view2.hashCode());
    }
}
