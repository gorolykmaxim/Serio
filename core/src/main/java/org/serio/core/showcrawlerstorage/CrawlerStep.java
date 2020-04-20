package org.serio.core.showcrawlerstorage;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;

/**
 * {@link Crawler} building block.
 *
 * <p>A singular crawler instruction, that accepts an input data from the previously executed step, performs
 * it's own computation / input data transformation, and forwards the result to the next step.</p>
 */
public class CrawlerStep {
    private final Type type;
    private final Map<String, String> attributes;

    /**
     * @see CrawlerStep#CrawlerStep(Type, Map)
     */
    public CrawlerStep(Type type) {
        this(type, null);
    }

    /**
     * Construct a crawler step.
     *
     * @param type type of the computation, that will be performed by this step
     * @param attributes additional parameters, that configure the step's computation. Each {@link Type} has it's
     *                   own set of unique attributes.
     */
    public CrawlerStep(Type type, Map<String, String> attributes) {
        if (type == null) {
            throw new IllegalArgumentException("Crawl step type is not specified");
        }
        this.type = type;
        this.attributes = attributes != null ? new HashMap<>(attributes) : new HashMap<>();
    }

    /**
     * Get type of the crawler step.
     *
     * @return type of this step
     */
    public Type getType() {
        return type;
    }

    /**
     * Get value of the attribute with the specified name.
     *
     * @param attributeName name of the attribute
     * @return possible value of the attribute if specified
     */
    public Optional<String> getAttribute(String attributeName) {
        return Optional.ofNullable(attributes.get(attributeName));
    }

    /**
     * Specify value of the specified attribute of the crawler step.
     *
     * <p>This operation produces a new crawler step, while preserving the original one as is.</p>
     *
     * @param name name of the attribute to set
     * @param value value of the attribute to set
     * @return crawler step with the new attribute set
     */
    public CrawlerStep setAttribute(String name, String value) {
        Map<String, String> attributes = new HashMap<>(this.attributes);
        attributes.put(name, value);
        return new CrawlerStep(type, attributes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlerStep that = (CrawlerStep) o;
        return type == that.type &&
                Objects.equals(attributes, that.attributes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(type, attributes);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlerStep{" +
                "type=" + type +
                ", attributes=" + attributes +
                '}';
    }

    /**
     * Possible types of a {@link CrawlerStep}.
     */
    public enum Type {
        /**
         * Return value of the attribute called "value" as a result of this step while ignoring any input data
         * from previous steps.
         */
        value,
        /**
         * Treat array of input strings from the previous steps as URLs and fetch their contents. Return a corresponding
         * array of raw string response bodies.
         */
        fetch,
        /**
         * Insert each string from the input array of strings into the string-template, defined in the attribute
         * named "template". Works like a sprintf() or a {@link String#format(String, Object...)}, where the first
         * argument is the string-template. This format will be applied to each string from the input string array.
         * For example if the template is "Episode %d" and an array of input strings is ["1", "2", "3"], then
         * the result of this step will be: ["Episode 1", "Episode 2", "Episode 3"].
         */
        transform,
        /**
         * Apply the regular expression, defined in the attribute named "regexp", to each string from the input array
         * find all the matches across all of the input strings and return all of them as a singular flat
         * array of results.
         */
        regexp
    }
}
