package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.showscrawler.CrawlingResult;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.List;
import java.util.Objects;

/**
 * Event that will trigger rendering of {@link ViewIds#CRAWL_PREVIEW} view.
 */
public class CrawlPreviewEvent implements ApplicationEvent {
    private final String crawlerType;
    private final CrawlingResult crawlingResult;

    /**
     * Construct an event.
     *
     * @param crawlerType type of the crawler that is being previewed. Can't be null.
     * @param crawlingResult result of the crawler's execution, that will be displayed. Can't be null.
     */
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

    /**
     * Get type of the crawler, crawling of which is being previewed.
     *
     * @return crawler type
     */
    public String getCrawlerType() {
        return crawlerType;
    }

    /**
     * Get list of strings, crawled by the previewed crawler.
     *
     * @return crawlers output date
     */
    public List<String> getCrawlItems() {
        return crawlingResult.getOutput();
    }

    /**
     * Get the whole crawling result of the previewed crawler.
     *
     * @return whole crawling result
     */
    public CrawlingResult getCrawlingResult() {
        return crawlingResult;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.CRAWL_PREVIEW;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CrawlPreviewEvent that = (CrawlPreviewEvent) o;
        return Objects.equals(crawlerType, that.crawlerType) &&
                Objects.equals(crawlingResult, that.crawlingResult);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(crawlerType, crawlingResult);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "CrawlPreviewEvent{" +
                "crawlerType='" + crawlerType + '\'' +
                ", crawlingResult=" + crawlingResult +
                '}';
    }
}
