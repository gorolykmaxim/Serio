package org.serio.core.showstorage;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class EpisodeTest {
    private String videoUrl;

    @Before
    public void setUp() throws Exception {
        videoUrl = "http://localhost:8081/video.avi";
    }

    @Test
    public void shouldHaveIdAsItsPositionalNumberInItsShow() {
        // when
        Episode episode = new Episode(1, videoUrl);
        // then
        assertEquals(1, episode.getId());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveItsIdEqualToZero() {
        // when
        new Episode(0, videoUrl);
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveVideoUrlNotSpecified() {
        // when
        new Episode(1, null);
    }

    @Test
    public void shouldHaveSpecifiedName() {
        // given
        String name = "The day after tomorrow";
        // when
        Episode episode = new Episode(1, name, videoUrl);
        // then
        assertEquals(name, episode.getName());
    }

    @Test
    public void showHaveGenericNameWithItsPositionInAShowIfNoNameWereSpecified() {
        // when
        Episode episode = new Episode(1, videoUrl);
        // then
        assertEquals("Episode " + 1, episode.getName());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // when
        Episode episode1 = new Episode(1, videoUrl);
        Episode episode2 = new Episode(1, videoUrl);
        // then
        assertEquals(episode1, episode2);
        assertEquals(episode1.hashCode(), episode2.hashCode());
    }

    @Test
    public void shouldNotBeEqualSinceAttributesAreDifferent() {
        // when
        Episode episode1 = new Episode(1, videoUrl);
        Episode episode2 = new Episode(2, videoUrl);
        // then
        assertNotEquals(episode1, episode2);
        assertNotEquals(episode1.hashCode(), episode2.hashCode());
    }
}
