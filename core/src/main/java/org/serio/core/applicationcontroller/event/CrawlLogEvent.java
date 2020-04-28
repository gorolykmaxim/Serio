package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Optional;

public class CrawlLogEvent implements ApplicationEvent {
    private final String crawlerType;
    private final List<IndexedCrawlLogEntry> logEntries;

    public CrawlLogEvent(List<CrawlLogEntry> logEntries) {
        this(null, logEntries);
    }

    public CrawlLogEvent(String crawlerType, List<CrawlLogEntry> logEntries) {
        this.crawlerType = crawlerType;
        this.logEntries = new ArrayList<>(logEntries.size());
        for (int i = 0; i < logEntries.size(); i++) {
            this.logEntries.add(new IndexedCrawlLogEntry(i, logEntries.get(i)));
        }

    }

    public Optional<String> getCrawlerType() {
        return Optional.ofNullable(crawlerType);
    }

    public List<IndexedCrawlLogEntry> getLogEntries() {
        return logEntries;
    }

    @Override
    public int getViewId() {
        return ViewIds.CRAWL_LOG;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlLogEvent that = (CrawlLogEvent) o;
        return Objects.equals(crawlerType, that.crawlerType) &&
                Objects.equals(logEntries, that.logEntries);
    }

    @Override
    public int hashCode() {
        return Objects.hash(crawlerType, logEntries);
    }

    @Override
    public String toString() {
        return "CrawlLogEvent{" +
                "crawlerType='" + crawlerType + '\'' +
                ", logEntries=" + logEntries +
                '}';
    }
}
