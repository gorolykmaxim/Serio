package org.serio.core.showscrawler.crawler.step;

import java.util.Objects;

/**
 * Insert each string from the input array of strings into the string-template.
 *
 * <p>Works like a sprintf() or a {@link String#format(String, Object...)}, where the first argument is
 * the string-template. This format will be applied to each string from the input string array.
 * For example if the template is "Episode %d" and an array of input strings is ["1", "2", "3"], then
 * the result of this step will be: ["Episode 1", "Episode 2", "Episode 3"].</p>
 */
public class TransformStep implements CrawlerStep {
    public static final String TYPE = "transform";

    private final String template;

    /**
     * Construct a "transform" step.
     *
     * @param template template to be applied to input data
     */
    public TransformStep(String template) {
        if (template == null) {
            throw new CrawlerStepAttributeException(TYPE, "template");
        }
        this.template = template;
    }

    /**
     * Get template to be applied to the input data during this transformation step.
     *
     * @return string template to be applied
     */
    public String getTemplate() {
        return template;
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
        TransformStep that = (TransformStep) o;
        return Objects.equals(template, that.template);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(template);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "TransformStep{" +
                "template='" + template + '\'' +
                '}';
    }
}
