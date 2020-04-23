package org.serio.core.showscrawler.crawler.step;

/**
 * {@link org.serio.core.showscrawler.crawler.Crawler} building block.
 *
 * <p>A singular crawler instruction, that accepts an input data from the previously executed step, performs
 * it's own computation / input data transformation, and forwards the result to the next step.</p>
 */
public interface CrawlerStep {

    /**
     * Get type of the crawler step.
     *
     * @return type of this step
     */
    String getType();
}
