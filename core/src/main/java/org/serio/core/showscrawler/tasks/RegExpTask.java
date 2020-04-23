package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.step.RegExpStep;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 * Executes a {@link RegExpStep}.
 *
 * @see RegExpStep
 */
public class RegExpTask implements CrawlerStepTask {
    private final RegExpStep step;

    /**
     * Construct a task.
     *
     * @param step crawler step to execute
     */
    public RegExpTask(RegExpStep step) {
        this.step = step;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(List<String> input) {
        try {
            Pattern pattern = Pattern.compile(step.getRegExp());
            return input
                    .stream()
                    .map(pattern::matcher)
                    .map(matcher -> {
                        List<String> results = new ArrayList<>();
                        while (matcher.find()) {
                            if (matcher.groupCount() == 0) {
                                // If there are no groups except for a zero-group, that means that the pattern does not
                                // include groups. In such case we will just add a zero-group to the results.
                                results.add(matcher.group(0));
                            } else {
                                // If there are groups in the match, that means that the pattern has groups thus we will
                                // append only such groups to the results, ignoring the zero group: we assume that
                                // the pattern, that has groups, is not interested in the zero-group.
                                for (int i = 1; i <= matcher.groupCount(); i++) {
                                    results.add(matcher.group(i));
                                }
                            }
                        }
                        return results;
                    })
                    .flatMap(List::stream)
                    .collect(Collectors.toList());
        } catch (Exception e) {
            throw new CrawlerStepTaskException(step, e);
        }
    }
}
