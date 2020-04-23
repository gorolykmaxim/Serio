package org.serio.core.showscrawler.crawler.step;

import java.util.Objects;

/**
 * Return specified value as a result of this step while ignoring any input data from previous steps.
 */
public class ValueStep implements CrawlerStep {
    public static final String TYPE = "value";

    private final String value;

    /**
     * Construct a "value" step.
     *
     * @param value value to be returned as a result of this step
     */
    public ValueStep(String value) {
        if (value == null) {
            throw new CrawlerStepAttributeException(TYPE, "value");
        }
        this.value = value;
    }

    /**
     * Get value, that will be returned as a result of this step.
     *
     * @return value to return
     */
    public String getValue() {
        return value;
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
        ValueStep valueStep = (ValueStep) o;
        return Objects.equals(value, valueStep.value);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(value);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "ValueStep{" +
                "value='" + value + '\'' +
                '}';
    }
}
