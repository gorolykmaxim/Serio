package org.serio.core.showscrawler;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;

import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * Result of execution of a single crawler.
 */
public class CrawlingResult {
    private final List<String> output;
    private final List<CrawlLogEntry> log;

    /**
     * Construct a crawling result.
     *
     * @param output actual output data, produced by the crawler
     * @param log the log, produced during by the crawler
     */
    public CrawlingResult(List<String> output, List<CrawlLogEntry> log) {
        this.output = Collections.unmodifiableList(output);
        this.log = Collections.unmodifiableList(log);
    }

    /**
     * Get first line of the result's output.
     *
     * @return first line of the result's output or null, if the output is empty.
     */
    public String getFirstOutputLine() {
        return output.isEmpty() ? null : output.get(0);
    }

    /**
     * Get output, produced by the crawler.
     *
     * @return complete crawler output data
     */
    public List<String> getOutput() {
        return output;
    }

    /**
     * Get complete log, produced by the crawler.
     *
     * @return crawling log
     */
    public List<CrawlLogEntry> getLog() {
        return log;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlingResult that = (CrawlingResult) o;
        return Objects.equals(output, that.output) &&
                Objects.equals(log, that.log);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(output, log);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlingResult{" +
                "output=" + output +
                ", log=" + log +
                '}';
    }
}
