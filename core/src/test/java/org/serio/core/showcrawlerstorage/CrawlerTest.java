package org.serio.core.showcrawlerstorage;

import org.junit.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.Assert.*;

public class CrawlerTest {
    @Test
    public void shouldCreateCrawlerWithNoSteps() {
        // when
        Crawler crawler = new Crawler();
        // then
        assertTrue(crawler.getSteps().isEmpty());
    }

    @Test
    public void shouldCreateCrawlerWithTheSpecifiedSteps() {
        // given
        List<CrawlerStep> steps = Arrays.asList(
                new CrawlerStep(CrawlerStep.Type.fetch),
                new CrawlerStep(CrawlerStep.Type.regexp)
        );
        // when
        Crawler crawler = new Crawler(steps);
        // then
        assertEquals(steps, crawler.getSteps());
    }

    @Test
    public void shouldCopyACrawlerWithANewStepAdded() {
        // given
        Crawler crawler = new Crawler();
        // when
        Crawler crawlerWithSteps = crawler.addStep(new CrawlerStep(CrawlerStep.Type.value));
        // then
        List<CrawlerStep> expectedSteps = Collections.singletonList(new CrawlerStep(CrawlerStep.Type.value));
        assertEquals(expectedSteps, crawlerWithSteps.getSteps());
    }

    @Test
    public void shouldBeEqualAndHaveTheSameHash() {
        // given
        List<CrawlerStep> steps = Collections.singletonList(new CrawlerStep(CrawlerStep.Type.fetch));
        // when
        Crawler crawler1 = new Crawler(steps);
        Crawler crawler2 = new Crawler(steps);
        // then
        assertEquals(crawler1, crawler2);
        assertEquals(crawler1.hashCode(), crawler2.hashCode());
    }

    @Test
    public void shouldNotBeEqual() {
        // when
        Crawler crawler1 = new Crawler(Collections.singletonList(new CrawlerStep(CrawlerStep.Type.value)));
        Crawler crawler2 = new Crawler();
        // then
        assertNotEquals(crawler1, crawler2);
        assertNotEquals(crawler1.hashCode(), crawler2.hashCode());
    }
}
