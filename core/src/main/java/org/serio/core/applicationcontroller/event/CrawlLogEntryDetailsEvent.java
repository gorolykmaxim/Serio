package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

/**
 * Event that will trigger rendering of {@link ViewIds#CRAWL_LOG_ENTRY_DETAILS} view.
 */
public class CrawlLogEntryDetailsEvent implements ApplicationEvent {
    private final IndexedCrawlLogEntry logEntry;

    /**
     * Construct an event.
     *
     * @param logEntry crawl log entry to be rendered. Can't be null.
     */
    public CrawlLogEntryDetailsEvent(IndexedCrawlLogEntry logEntry) {
        if (logEntry == null) {
            throw new IllegalArgumentException("Log entry is not specified");
        }
        this.logEntry = logEntry;
    }

    /**
     * Get craw log entry, that should be rendered.
     *
     * @return log entry to render
     */
    public IndexedCrawlLogEntry getLogEntry() {
        return logEntry;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.CRAWL_LOG_ENTRY_DETAILS;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlLogEntryDetailsEvent that = (CrawlLogEntryDetailsEvent) o;
        return Objects.equals(logEntry, that.logEntry);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(logEntry);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlLogEntryDetailsEvent{" +
                "logEntry=" + logEntry +
                '}';
    }
}
