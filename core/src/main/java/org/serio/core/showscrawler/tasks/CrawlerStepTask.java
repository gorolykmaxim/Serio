package org.serio.core.showscrawler.tasks;

import java.util.List;

/**
 * A task that actually executes a corresponding {@link org.serio.core.showscrawler.crawler.step.CrawlerStep}.
 */
public interface CrawlerStepTask {
    /**
     * Process the specified input data according to the actual crawler step rules and return processed results.
     *
     * @param input input to process during this step
     * @return processed input data
     */
    List<String> execute(List<String> input);
}
