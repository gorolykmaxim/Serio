package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.showscrawler.CrawlingResult;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.List;
import java.util.Objects;

public class CrawlPreviewEvent implements ApplicationEvent {
    private final String crawlerType;
    private final CrawlingResult crawlingResult;

    public CrawlPreviewEvent(String crawlerType, CrawlingResult crawlingResult) {
        if (StringUtils.isEmpty(crawlerType)) {
            throw new IllegalArgumentException("Crawler type is empty or not specified");
        }
        if (crawlingResult == null) {
            throw new IllegalArgumentException("Crawling result is not specified");
        }
        this.crawlerType = crawlerType;
        this.crawlingResult = crawlingResult;
    }

    public String getCrawlerType() {
        return crawlerType;
    }

    public List<String> getCrawlItems() {
        return crawlingResult.getOutput();
    }

    public CrawlingResult getCrawlingResult() {
        return crawlingResult;
    }

    @Override
    public int getViewId() {
        return ViewIds.CRAWL_PREVIEW;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlPreviewEvent that = (CrawlPreviewEvent) o;
        return Objects.equals(crawlerType, that.crawlerType) &&
                Objects.equals(crawlingResult, that.crawlingResult);
    }

    @Override
    public int hashCode() {
        return Objects.hash(crawlerType, crawlingResult);
    }

    @Override
    public String toString() {
        return "CrawlPreviewEvent{" +
                "crawlerType='" + crawlerType + '\'' +
                ", crawlingResult=" + crawlingResult +
                '}';
    }
}
