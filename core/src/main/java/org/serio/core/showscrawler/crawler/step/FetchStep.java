package org.serio.core.showscrawler.crawler.step;

import java.util.Objects;

/**
 * Treats array of input strings from the previous steps as URLs and fetches their contents. Returns a corresponding
 * array of raw string response bodies.
 */
public class FetchStep implements CrawlerStep {
    public static final String TYPE = "fetch";

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
        return o != null && getClass() == o.getClass();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hashCode(TYPE);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "FetchStep{}";
    }
}
