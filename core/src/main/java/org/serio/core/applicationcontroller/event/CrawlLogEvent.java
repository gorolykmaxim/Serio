package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Optional;

/**
 * Event that will trigger rendering of {@link ViewIds#CRAWL_LOG} view.
 */
public class CrawlLogEvent implements ApplicationEvent {
    private final String crawlerType;
    private final List<IndexedCrawlLogEntry> logEntries;

    /**
     * Construct an event, that will display last crawl log of a show.
     *
     * @param logEntries log entries to display
     */
    public CrawlLogEvent(List<CrawlLogEntry> logEntries) {
        this(null, logEntries);
    }

    /**
     * Construct an event, that will display the crawl log that was generated while previewing a crawler of
     * the specified type.
     *
     * @param crawlerType type of the crawler, that has generated the log
     * @param logEntries log entries to display
     */
    public CrawlLogEvent(String crawlerType, List<CrawlLogEntry> logEntries) {
        this.crawlerType = crawlerType;
        this.logEntries = new ArrayList<>(logEntries.size());
        for (int i = 0; i < logEntries.size(); i++) {
            this.logEntries.add(new IndexedCrawlLogEntry(i, logEntries.get(i)));
        }

    }

    /**
     * Get type of the crawler, that has generated the log.
     *
     * @return crawler type. Can be empty, if the event is going to display a show's last crawling log.
     */
    public Optional<String> getCrawlerType() {
        return Optional.ofNullable(crawlerType);
    }

    /**
     * Get entries of the log.
     *
     * @return log entries to display
     */
    public List<IndexedCrawlLogEntry> getLogEntries() {
        return logEntries;
    }

    /**
     * Get entry from the event's log by it's ID.
     *
     * @param id ID of the log entry
     * @return entry of the event's log with the specified ID. Can be empty if the log does not contain such entry.
     */
    public Optional<IndexedCrawlLogEntry> getLogEntryById(int id) {
        if (id < 0 || id >= logEntries.size()) {
            return Optional.empty();
        } else {
            return Optional.of(logEntries.get(id));
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.CRAWL_LOG;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlLogEvent that = (CrawlLogEvent) o;
        return Objects.equals(crawlerType, that.crawlerType) &&
                Objects.equals(logEntries, that.logEntries);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(crawlerType, logEntries);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlLogEvent{" +
                "crawlerType='" + crawlerType + '\'' +
                ", logEntries=" + logEntries +
                '}';
    }
}
