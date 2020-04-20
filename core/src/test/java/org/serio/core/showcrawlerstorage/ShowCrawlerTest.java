package org.serio.core.showcrawlerstorage;

import org.junit.Test;

import java.util.Collections;

import static org.junit.Assert.*;

public class ShowCrawlerTest {
    @Test(expected = IllegalArgumentException.class)
    public void shouldNotCreateShowCrawlerWithoutEpisodeVideoCrawler() {
        // when
        new ShowCrawler(null, null, null);
    }

    @Test
    public void shouldCreateShowCrawlerWithTheSpecifiedEpisodeVideoCrawler() {
        // when
        Crawler episodeVideoCrawler = new Crawler();
        ShowCrawler showCrawler = new ShowCrawler(null, episodeVideoCrawler, null);
        // then
        assertEquals(episodeVideoCrawler, showCrawler.getEpisodeVideoCrawler());
    }

    @Test
    public void shouldCreateShowCrawlerWithoutThumbnailCrawlerSpecified() {
        // when
        ShowCrawler showCrawler = new ShowCrawler(null, new Crawler(), null);
        // then
        assertTrue(showCrawler.getThumbnailCrawler().getSteps().isEmpty());
    }

    @Test
    public void shouldCreateShowCrawlerWithThumbnailCrawlerSpecified() {
        // when
        Crawler thumbnailCrawler = new Crawler().addStep(new CrawlerStep(CrawlerStep.Type.fetch));
        ShowCrawler showCrawler = new ShowCrawler(thumbnailCrawler, new Crawler(), null);
        // then
        assertEquals(thumbnailCrawler, showCrawler.getThumbnailCrawler());
    }

    @Test
    public void shouldCreateShowCrawlerWithoutEpisodeNameCrawlerSpecified() {
        // when
        ShowCrawler showCrawler = new ShowCrawler(null, new Crawler(), null);
        // then
        assertTrue(showCrawler.getEpisodeNameCrawler().getSteps().isEmpty());
    }

    @Test
    public void shouldCreateShowCrawlerWithEpisodeNameCrawlerSpecified() {
        // when
        Crawler episodeNameCrawler = new Crawler().addStep(new CrawlerStep(CrawlerStep.Type.transform));
        ShowCrawler showCrawler = new ShowCrawler(null, new Crawler(), episodeNameCrawler);
        // then
        assertEquals(episodeNameCrawler, showCrawler.getEpisodeNameCrawler());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // given
        Crawler thumbnailCrawler = new Crawler()
                .addStep(new CrawlerStep(CrawlerStep.Type.fetch))
                .addStep(new CrawlerStep(CrawlerStep.Type.regexp));
        Crawler episodeVideoCrawler = new Crawler()
                .addStep(new CrawlerStep(CrawlerStep.Type.fetch))
                .addStep(new CrawlerStep(CrawlerStep.Type.regexp));
        Crawler episodeNameCrawler = new Crawler(Collections.singletonList(new CrawlerStep(CrawlerStep.Type.transform)));
        // when
        ShowCrawler showCrawler1 = new ShowCrawler(thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler);
        ShowCrawler showCrawler2 = new ShowCrawler(thumbnailCrawler, episodeVideoCrawler, episodeNameCrawler);
        // then
        assertEquals(showCrawler1, showCrawler2);
        assertEquals(showCrawler1.hashCode(), showCrawler2.hashCode());
    }

    @Test
    public void shouldNotBeEqual() {
        // when
        ShowCrawler showCrawler1 = new ShowCrawler(new Crawler(Collections.singletonList(new CrawlerStep(CrawlerStep.Type.fetch))), new Crawler(), null);
        ShowCrawler showCrawler2 = new ShowCrawler(null, new Crawler(), new Crawler());
        // then
        assertNotEquals(showCrawler1, showCrawler2);
        assertNotEquals(showCrawler1.hashCode(), showCrawler2.hashCode());
    }
}
