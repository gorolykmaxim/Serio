package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.CrawlerStep;

import java.util.List;
import java.util.stream.Collectors;

/**
 * Executes a {@link CrawlerTask} with a "transform" type.
 *
 * <p>Insert each string from the input array of strings into the string-template.</p>
 *
 * <p>Works like a sprintf() or a {@link String#format(String, Object...)}, where the first argument is
 * the string-template. This format will be applied to each string from the input string array.
 * For example if the template is "Episode %d" and an array of input strings is ["1", "2", "3"], then
 * the result of this step will be: ["Episode 1", "Episode 2", "Episode 3"].</p>
 */
public class TransformTask implements CrawlerStepTask {
    /**
     * Name of the property of this step, that contains the string-template.
     */
    public static final String TEMPLATE = "template";

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(CrawlerStep step, List<String> input) {
        return input
                .stream()
                .map(item -> String.format(step.getProperty(TEMPLATE).get(), item))
                .collect(Collectors.toList());
    }
}
