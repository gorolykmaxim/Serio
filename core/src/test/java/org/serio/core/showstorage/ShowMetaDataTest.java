package org.serio.core.showstorage;

import org.junit.Before;
import org.junit.Test;

import java.util.*;

import static org.junit.Assert.*;
import static org.junit.Assert.assertNotEquals;

public class ShowMetaDataTest {
    private UUID showId;
    private String showName;
    private String thumbnailUrl;

    @Before
    public void setUp() throws Exception {
        showId = UUID.randomUUID();
        showName = "The Office";
        thumbnailUrl = "http://localhost:3000/thumbnail.png";
    }

    @Test
    public void shouldHaveIdGeneratedFromTheNameWhileBeingCreatedForTheFirstTime() {
        // given
        UUID expectedId = UUID.nameUUIDFromBytes(showName.getBytes());
        // when
        ShowMetaData metaData = new ShowMetaData(null, showName, thumbnailUrl);
        // then
        assertEquals(expectedId, metaData.getId());
    }

    @Test(expected = IllegalArgumentException.class)
    public void shouldNotHaveAnEmptyName() {
        // when
        new ShowMetaData(showId, null, thumbnailUrl);
    }

    @Test
    public void shouldHaveSpecifiedThumbnailUrl() {
        // when
        ShowMetaData metaData = new ShowMetaData(showId, showName, thumbnailUrl);
        // then
        assertEquals(thumbnailUrl, metaData.getThumbnailUrl());
    }

    @Test
    public void shouldNotHaveANullThumbnailUrlEvenIfItWasNotSpecified() {
        // when
        ShowMetaData metaData = new ShowMetaData(showId, showName, null);
        // then
        assertNotNull(metaData.getThumbnailUrl());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // given
        ShowMetaData metaData1 = new ShowMetaData(
                showId,
                showName,
                thumbnailUrl
        );
        ShowMetaData metaData2 = new ShowMetaData(
                showId,
                showName,
                thumbnailUrl
        );
        // then
        assertEquals(metaData1, metaData2);
        assertEquals(metaData1.hashCode(), metaData2.hashCode());
    }

    @Test
    public void shouldNotBeEqualSinceAttributesAreDifferent() {
        // given
        ShowMetaData metaData1 = new ShowMetaData(showId, showName, thumbnailUrl);
        ShowMetaData metaData2 = new ShowMetaData(showId, showName + "suffix", thumbnailUrl + "suffix");
        // then
        assertNotEquals(metaData1, metaData2);
        assertNotEquals(metaData1.hashCode(), metaData2.hashCode());
    }
}
