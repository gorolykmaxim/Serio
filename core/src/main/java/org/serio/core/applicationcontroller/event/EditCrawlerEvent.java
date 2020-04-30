package org.serio.core.applicationcontroller.event;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.Objects;

/**
 * Event that will trigger rendering of {@link ViewIds#EDIT_CRAWLER} view.
 */
public class EditCrawlerEvent implements ApplicationEvent {
    private final String crawlerType;
    private final String crawler;

    /**
     * Construct an event.
     *
     * @param crawlerType type of the crawler, that is being edited. Can't be null.
     * @param crawler the actual crawler's body, that is being edited. Can be null or empty if the crawler is being
     *                created for the first time.
     */
    public EditCrawlerEvent(String crawlerType, String crawler) {
        if (StringUtils.isEmpty(crawlerType)) {
            throw new IllegalArgumentException("Crawler type is empty or not specified");
        }
        this.crawlerType = crawlerType;
        this.crawler = StringUtils.defaultString(crawler);
    }

    /**
     * Get type of the crawler being edited.
     *
     * @return crawler type
     */
    public String getCrawlerType() {
        return crawlerType;
    }

    /**
     * Get body of the crawler being edited.
     *
     * @return crawler's body or an empty string if the crawler is getting created for the first time
     */
    public String getCrawler() {
        return crawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.EDIT_CRAWLER;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EditCrawlerEvent that = (EditCrawlerEvent) o;
        return Objects.equals(crawlerType, that.crawlerType) &&
                Objects.equals(crawler, that.crawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(crawlerType, crawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "EditCrawler{" +
                "crawlerType='" + crawlerType + '\'' +
                ", crawler='" + crawler + '\'' +
                '}';
    }
}
