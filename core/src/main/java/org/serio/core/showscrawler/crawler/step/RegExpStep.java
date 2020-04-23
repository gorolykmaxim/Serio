package org.serio.core.showscrawler.crawler.step;

import java.util.Objects;

/**
 * Applies the regular expression to each string from the input array, finds all the matches across all of
 * the input strings and returns all of them as a singular flat array of results.
 *
 * <p>If the regular expression of this step utilizes groups (e.g. "()"), will only return those matched groups
 * without returning an actual complete match. In other case when there are no such groups specified in the
 * expression - the actual expression match will be returned instead.</p>
 */
public class RegExpStep implements CrawlerStep {
    public static final String TYPE = "regExp";

    private final String regExp;

    /**
     * Construct a "regExp" step.
     *
     * @param regExp regular expression to be executed against the input data
     */
    public RegExpStep(String regExp) {
        if (regExp == null) {
            throw new CrawlerStepAttributeException(TYPE, "regExp");
        }
        this.regExp = regExp;
    }

    /**
     * Get regular expression to be executed during this step.
     *
     * @return step's regular expression
     */
    public String getRegExp() {
        return regExp;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getType() {
        return TYPE;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RegExpStep that = (RegExpStep) o;
        return Objects.equals(regExp, that.regExp);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(regExp);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "RegExpStep{" +
                "regExp='" + regExp + '\'' +
                '}';
    }
}
