package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

public class EditCrawlerEvent implements ApplicationEvent {
    private final String crawlerType;
    private final String crawler;

    public EditCrawlerEvent(String crawlerType, String crawler) {
        if (StringUtils.isEmpty(crawlerType)) {
            throw new IllegalArgumentException("Crawler type is empty or not specified");
        }
        this.crawlerType = crawlerType;
        this.crawler = StringUtils.defaultString(crawler);
    }

    public String getCrawlerType() {
        return crawlerType;
    }

    public String getCrawler() {
        return crawler;
    }

    @Override
    public int getViewId() {
        return ViewIds.EDIT_CRAWLER;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EditCrawlerEvent that = (EditCrawlerEvent) o;
        return Objects.equals(crawlerType, that.crawlerType) &&
                Objects.equals(crawler, that.crawler);
    }

    @Override
    public int hashCode() {
        return Objects.hash(crawlerType, crawler);
    }

    @Override
    public String toString() {
        return "EditCrawler{" +
                "crawlerType='" + crawlerType + '\'' +
                ", crawler='" + crawler + '\'' +
                '}';
    }
}
