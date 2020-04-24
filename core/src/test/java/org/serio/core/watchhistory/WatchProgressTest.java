package org.serio.core.watchhistory;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class WatchProgressTest {
    @Test
    public void shouldCreateAWatchProgressWithTheSpecifiedPercentage() {
        // given
        double percentage = 15.32456;
        // when
        WatchProgress progress = WatchProgress.fromPercentage(percentage);
        // then
        assertEquals(percentage, progress.getPercentage(), 0.1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotCreatAWatchProgressWithANegativePercentage() {
        // when
        WatchProgress.fromPercentage(-1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotCreateAWatchProgressWithAPercentageHigherThan100() {
        // when
        WatchProgress.fromPercentage(101);
    }

    @Test
    public void shouldBeEqualAndHaveSameHash() {
        // when
        WatchProgress progress1 = WatchProgress.fromPercentage(100);
        WatchProgress progress2 = WatchProgress.COMPLETE;
        // then
        assertEquals(progress1, progress2);
        assertEquals(progress1.hashCode(), progress2.hashCode());
    }

    @Test
    public void shouldNotBeEqual() {
        // when
        WatchProgress progress1 = WatchProgress.NONE;
        WatchProgress progress2 = WatchProgress.COMPLETE;
        // then
        assertNotEquals(progress1, progress2);
        assertNotEquals(progress1.hashCode(), progress2.hashCode());
    }
}
