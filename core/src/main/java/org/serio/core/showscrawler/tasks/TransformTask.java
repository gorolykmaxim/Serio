package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.step.TransformStep;

import java.util.List;
import java.util.stream.Collectors;

/**
 * Executes a {@link TransformStep}.
 *
 * @see TransformStep
 */
public class TransformTask implements CrawlerStepTask {
    private final TransformStep step;

    /**
     * Construct a task.
     *
     * @param step crawler step to execute
     */
    public TransformTask(TransformStep step) {
        this.step = step;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(List<String> input) {
        return input.stream().map(item -> String.format(step.getTemplate(), item)).collect(Collectors.toList());
    }
}
