package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.*;

public class EditShowCrawlerEvent implements ApplicationEvent {
    private final UUID showId;
    private final String showName;
    private final Map<String, String> crawlerTypeToCrawler;

    public EditShowCrawlerEvent() {
        this(null, null, null, null, null);
    }

    public EditShowCrawlerEvent(UUID showId, String showName, String thumbnailCrawler, String episodeVideoCrawler,
                                String episodeNameCrawler) {
        this.showId = showId;
        this.showName = showName;
        crawlerTypeToCrawler = new HashMap<>();
        crawlerTypeToCrawler.put(CrawlerTypes.THUMBNAIL, thumbnailCrawler);
        crawlerTypeToCrawler.put(CrawlerTypes.EPISODE_VIDEO, episodeVideoCrawler);
        crawlerTypeToCrawler.put(CrawlerTypes.EPISODE_NAME, episodeNameCrawler);
    }

    private EditShowCrawlerEvent(UUID showId, String showName, Map<String, String> crawlerTypeToCrawler) {
        this.showId = showId;
        this.showName = showName;
        this.crawlerTypeToCrawler = Collections.unmodifiableMap(crawlerTypeToCrawler);
    }

    public EditShowCrawlerEvent setCrawler(String crawlerType, String crawler) {
        Map<String, String> updatedCrawlerTypeToCrawler = new HashMap<>(crawlerTypeToCrawler);
        updatedCrawlerTypeToCrawler.put(crawlerType, crawler);
        return new EditShowCrawlerEvent(showId, showName, updatedCrawlerTypeToCrawler);
    }

    public Optional<UUID> getShowId() {
        return Optional.ofNullable(showId);
    }

    public Optional<String> getShowName() {
        return Optional.ofNullable(showName);
    }

    public Optional<String> getCrawler(String crawlerType) {
        return Optional.ofNullable(crawlerTypeToCrawler.get(crawlerType));
    }

    @Override
    public int getViewId() {
        return ViewIds.EDIT_SHOW_CRAWLER;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EditShowCrawlerEvent event = (EditShowCrawlerEvent) o;
        return Objects.equals(showId, event.showId) &&
                Objects.equals(showName, event.showName) &&
                Objects.equals(crawlerTypeToCrawler, event.crawlerTypeToCrawler);
    }

    @Override
    public int hashCode() {
        return Objects.hash(showId, showName, crawlerTypeToCrawler);
    }

    @Override
    public String toString() {
        return "EditShowCrawlerEvent{" +
                "showId=" + showId +
                ", showName='" + showName + '\'' +
                ", crawlerTypeToCrawler=" + crawlerTypeToCrawler +
                '}';
    }
}
