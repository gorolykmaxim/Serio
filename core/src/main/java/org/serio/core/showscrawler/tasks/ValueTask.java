package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.CrawlerStep;

import java.util.Collections;
import java.util.List;

/**
 * Executes a {@link CrawlerTask} with a "value" type.
 *
 * <p>Return specified value as a result of this step while ignoring any input data from previous steps.</p>
 */
public class ValueTask implements CrawlerStepTask {
    /**
     * Name of the property of this step, that contains the value to return.
     */
    public static final String VALUE = "value";

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(CrawlerStep step, List<String> input) {
        return Collections.singletonList(step.getProperty(VALUE).get());
    }
}
