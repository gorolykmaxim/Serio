package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class CrawlLogEvent implements ApplicationEvent {
    private final String crawlerType;
    private final List<IndexedCrawlLogEntry> logEntries;

    public CrawlLogEvent(String crawlerType, List<CrawlLogEntry> logEntries) {
        if (StringUtils.isEmpty(crawlerType)) {
            throw new IllegalArgumentException("Crawler type is not specified or empty");
        }
        this.crawlerType = crawlerType;
        this.logEntries = new ArrayList<>(logEntries.size());
        for (int i = 0; i < logEntries.size(); i++) {
            this.logEntries.add(new IndexedCrawlLogEntry(i, logEntries.get(i)));
        }

    }

    public String getCrawlerType() {
        return crawlerType;
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
