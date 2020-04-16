package org.serio.core.watchhistory;

import org.junit.Before;
import org.junit.Test;

import java.time.LocalDate;
import java.util.UUID;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class ShowViewTest {
    private String showId;

    @Before
    public void setUp() throws Exception {
        showId = UUID.randomUUID().toString();
    }

    @Test
    public void shouldHaveSpecifiedShowId() {
        // when
        ShowView view = new ShowView(showId);
        // then
        assertEquals(showId, view.getShowId());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveShowIdNotSpecified() {
        // when
        new ShowView(null);
    }

    @Test
    public void shouldHaveSpecifiedLastWatchedDate() {
        // given
        LocalDate lastWatchedDate = LocalDate.now().minusDays(1);
        // when
        ShowView view = new ShowView(showId, lastWatchedDate);
        // then
        assertEquals(lastWatchedDate, view.getLastWatchedDate());
    }

    @Test
    public void shouldHaveLastWatchedDateSetToTodayIfNotSpecified() {
        // when
        ShowView view = new ShowView(showId);
        // then
        assertEquals(LocalDate.now(), view.getLastWatchedDate());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // when
        ShowView view1 = new ShowView(showId);
        ShowView view2 = new ShowView(showId);
        // then
        assertEquals(view1, view2);
        assertEquals(view1.hashCode(), view2.hashCode());
    }

    @Test
    public void shouldNotBeEqual() {
        // when
        ShowView view1 = new ShowView(showId);
        ShowView view2 = new ShowView(UUID.randomUUID().toString(), LocalDate.now().minusDays(1));
        // then
        assertNotEquals(view1, view2);
        assertNotEquals(view1.hashCode(), view2.hashCode());
    }
}
