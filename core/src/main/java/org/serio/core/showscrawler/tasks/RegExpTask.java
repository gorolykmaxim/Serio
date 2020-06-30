package org.serio.core.showscrawler.tasks;

import org.serio.core.showscrawler.crawler.CrawlerStep;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 * Executes a {@link CrawlerTask} with a "regExp" type.
 *
 * <p>Applies the regular expression to each string from the input array, finds all the matches across all of
 * the input strings and returns all of them as a singular flat array of results.</p>
 *
 * <p>If the regular expression of this step utilizes groups (e.g. "()"), will only return those matched groups
 * without returning an actual complete match. In other case when there are no such groups specified in the
 * expression - the actual expression match will be returned instead.</p>
 */
public class RegExpTask implements CrawlerStepTask {
    /**
     * Name of the property of this step, that contains the regular expression.
     */
    public static final String REG_EXP = "regExp";

    /**
     * {@inheritDoc}
     */
    @Override
    public List<String> execute(CrawlerStep step, List<String> input) {
        try {
            Pattern pattern = Pattern.compile(step.getProperty(REG_EXP).get());
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
