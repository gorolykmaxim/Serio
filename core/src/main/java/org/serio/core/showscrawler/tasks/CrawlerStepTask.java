package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.CrawlerStep;

import java.util.List;

/**
 * A task that actually executes a corresponding {@link org.serio.core.showscrawler.crawler.CrawlerStep}.
 */
public interface CrawlerStepTask {
    /**
     * Process the specified input data according to the actual crawler step rules and return processed results.
     *
     * @param step  step to be execute upon the specified input
     * @param input input to process during this step
     * @return processed input data
     */
    List<String> execute(CrawlerStep step, List<String> input);
}
