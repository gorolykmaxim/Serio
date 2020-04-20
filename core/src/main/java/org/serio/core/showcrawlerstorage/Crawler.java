package org.serio.core.showcrawlerstorage;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * {@link ShowCrawler} building block.
 *
 * <p>Used to crawler a part of show, such as a thumbnail or a list of episodes.</p>
 *
 * <p>Crawler is presented as a set of {@link CrawlerStep}s where the outputs of one step are getting passed
 * to the following step. Each step accepts a flat array of strings as an input and produces a flat array
 * of strings as an output.</p>
 */
public class Crawler {
    private final List<CrawlerStep> steps;

    /**
     * @see Crawler#Crawler(List)
     */
    public Crawler() {
        this(null);
    }

    /**
     * Construct a crawler.
     *
     * @param steps set of steps, that will be executed during the execution of this crawler
     */
    public Crawler(List<CrawlerStep> steps) {
        this.steps = steps != null ? new ArrayList<>(steps) : Collections.emptyList();
    }

    /**
     * Add step to this crawler.
     *
     * <p>This operation produces a new crawler, while preserving an original one as is.</p>
     *
     * @param step additional step to execute as a part of this crawler
     * @return a crawler with the specified step added
     */
    public Crawler addStep(CrawlerStep step) {
        List<CrawlerStep> steps = new ArrayList<>(this.steps);
        steps.add(step);
        return new Crawler(steps);
    }

    /**
     * Get collection of steps that represent this crawler.
     *
     * @return steps of this crawler
     */
    public List<CrawlerStep> getSteps() {
        return steps;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Crawler crawler = (Crawler) o;
        return Objects.equals(steps, crawler.steps);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(steps);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "Crawler{" +
                "steps=" + steps +
                '}';
    }
}
