package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

public class CrawlLogEntryDetailsEvent implements ApplicationEvent {
    private final IndexedCrawlLogEntry logEntry;

    public CrawlLogEntryDetailsEvent(IndexedCrawlLogEntry logEntry) {
        if (logEntry == null) {
            throw new IllegalArgumentException("Log entry is not specified");
        }
        this.logEntry = logEntry;
    }

    public IndexedCrawlLogEntry getLogEntry() {
        return logEntry;
    }

    @Override
    public int getViewId() {
        return ViewIds.CRAWL_LOG_ENTRY_DETAILS;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlLogEntryDetailsEvent that = (CrawlLogEntryDetailsEvent) o;
        return Objects.equals(logEntry, that.logEntry);
    }

    @Override
    public int hashCode() {
        return Objects.hash(logEntry);
    }

    @Override
    public String toString() {
        return "CrawlLogEntryDetailsEvent{" +
                "logEntry=" + logEntry +
                '}';
    }
}
