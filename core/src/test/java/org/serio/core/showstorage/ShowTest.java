package org.serio.core.showstorage;

import org.junit.Before;
import org.junit.Test;

import java.util.*;

import static org.junit.Assert.*;

public class ShowTest {
    private UUID showId;
    private String showName;
    private String thumbnailUrl;
    private int episodeId;
    private String videoUrl;

    @Before
    public void setUp() throws Exception {
        showId = UUID.randomUUID();
        showName = "The Office";
        thumbnailUrl = "http://localhost:3000/thumbnail.png";
        episodeId = 1;
        videoUrl = "http://localhost:3000/video.mp4";
    }

    @Test
    public void shouldHaveIdGeneratedFromTheNameWhileBeingCreatedForTheFirstTime() {
        // given
        UUID expectedId = UUID.nameUUIDFromBytes(showName.getBytes());
        // when
        Show show = Show.createNew(showName, Collections.emptyList());
        // then
        assertEquals(expectedId, show.getId());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveAnEmptyName() {
        // when
        Show.createNew(null, thumbnailUrl, Collections.emptyList());
    }

    @Test(expected = NullPointerException.class)
    public void showNotHaveEpisodesNotSpecified() {
        // when
        Show.createNew(showName, null);
    }

    @Test
    public void shouldHaveSpecifiedThumbnailUrl() {
        // when
        Show show = Show.createNew(showName, thumbnailUrl, Collections.emptyList());
        // then
        assertEquals(thumbnailUrl, show.getThumbnailUrl());
    }

    @Test
    public void shouldNotHaveANullThumbnailUrlEvenIfItWasNotSpecified() {
        // when
        Show show = Show.createNew(showName, Collections.emptyList());
        // then
        assertNotNull(show.getThumbnailUrl());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // given
        Show show1 = new Show(
                showId,
                showName,
                thumbnailUrl,
                Collections.singletonList(new Episode(episodeId, videoUrl))
        );
        Show show2 = new Show(
                showId,
                showName,
                thumbnailUrl,
                Collections.singletonList(new Episode(episodeId, videoUrl))
        );
        // then
        assertEquals(show1, show2);
        assertEquals(show1.hashCode(), show2.hashCode());
    }

    @Test
    public void shouldNotBeEqualSinceAttributesAreDifferent() {
        // given
        Show show1 = new Show(showId, showName, thumbnailUrl, Collections.emptyList());
        Show show2 = new Show(showId, showName + "suffix", thumbnailUrl + "suffix", Collections.emptyList());
        // then
        assertNotEquals(show1, show2);
        assertNotEquals(show1.hashCode(), show2.hashCode());
    }

    @Test
    public void shouldNotBeEqualSinceEpisodesAreDifferent() {
        // given
        Show show1 = new Show(
                showId,
                showName,
                thumbnailUrl,
                Collections.singletonList(new Episode(episodeId, videoUrl))
        );
        Show show2 = new Show(
                showId,
                showName,
                thumbnailUrl,
                Arrays.asList(
                        new Episode(episodeId, videoUrl),
                        new Episode(episodeId + 1, videoUrl)
                )
        );
        // then
        assertNotEquals(show1, show2);
        assertNotEquals(show1.hashCode(), show2.hashCode());
    }

    @Test
    public void shouldHaveItsOwnCopyOfTheSpecifiedListOfEpisodes() {
        // given
        List<Episode> episodes = new ArrayList<>();
        Show show = Show.createNew(showName, episodes);
        // when
        episodes.add(new Episode(1, videoUrl));
        // then
        assertNotEquals(episodes, show.getEpisodes());
    }

    @Test(expected = UnsupportedOperationException.class)
    public void shouldNotAllowModifyingItsEpisodesList() {
        // given
        Show show = Show.createNew(showName, new ArrayList<>());
        // when
        List<Episode> episodes = show.getEpisodes();
        episodes.add(new Episode(1, videoUrl));
    }
}
