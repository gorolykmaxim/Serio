package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.step.ValueStep;

import java.util.Collections;
import java.util.List;

/**
 * Executes a {@link ValueStep}.
 *
 * @see ValueStep
 */
public class ValueTask implements CrawlerStepTask {
    private final ValueStep step;

    /**
     * Construct a task.
     *
     * @param step crawler step to execute
     */
    public ValueTask(ValueStep step) {
        this.step = step;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(List<String> input) {
        return Collections.singletonList(step.getValue());
    }
}
