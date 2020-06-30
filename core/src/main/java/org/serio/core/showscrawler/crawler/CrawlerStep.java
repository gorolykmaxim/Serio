package org.serio.core.showscrawler.crawler;

import java.util.*;

/**
 * {@link Crawler} building block.
 *
 * <p>A singular crawler instruction, that accepts an input data from the previously executed step, performs
 * it's own computation / input data transformation, and forwards the result to the next step.</p>
 */
public class CrawlerStep {
    private final String type;
    private final Map<String, String> properties;

    /**
     * Construct a crawler step.
     *
     * @param type       type of the step
     * @param properties properties of the step
     */
    public CrawlerStep(String type, Map<String, String> properties) {
        this.type = type;
        this.properties = Collections.unmodifiableMap(new HashMap<>(properties));
    }

    /**
     * Get type of the crawler step.
     *
     * @return type of this step
     */
    public String getType() {
        return type;
    }

    /**
     * Get value of the specified step's property.
     *
     * @param propertyName name of the property
     * @return value of the specified property. If the step does not have the property specified - return
     * an empty optional.
     */
    public Optional<String> getProperty(String propertyName) {
        return Optional.ofNullable(properties.get(propertyName));
    }

    /**
     * Get all properties of this step.
     *
     * @return all properties of the step
     */
    public Map<String, String> getProperties() {
        return properties;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlerStep that = (CrawlerStep) o;
        return Objects.equals(type, that.type) &&
                Objects.equals(properties, that.properties);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(type, properties);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlerStep{" +
                "type='" + type + '\'' +
                ", properties=" + properties +
                '}';
    }
}
