package org.serio.core.applicationcontroller.event;

import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.ViewIds;

import java.util.*;

/**
 * Event that will trigger rendering of {@link ViewIds#EDIT_SHOW_CRAWLER} view.
 */
public class EditShowCrawlerEvent implements ApplicationEvent {
    private final UUID showId;
    private final String showName;
    private final Map<String, String> crawlerTypeToCrawler;

    /**
     * Construct an event that will display {@link ViewIds#EDIT_SHOW_CRAWLER} view to create a new show.
     */
    public EditShowCrawlerEvent() {
        this(null, null, null, null, null);
    }

    /**
     * Construct an event that will display {@link ViewIds#EDIT_SHOW_CRAWLER} view to edit an existing show.
     *
     * @param showId ID of the show, crawler of which is being edited
     * @param showName name of the show, crawler of which is being edited
     * @param thumbnailCrawler body of the thumbnail crawler of the edited show
     * @param episodeVideoCrawler body of the episode's video crawler of the edited show
     * @param episodeNameCrawler body of the episode's name crawler of the edited show
     */
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

    /**
     * Specify the name of the show, that is being edited.
     *
     * @param showName name of the show being edited
     * @return new instance of this event with the show name specified
     */
    public EditShowCrawlerEvent setShowName(String showName) {
        return new EditShowCrawlerEvent(showId, showName, crawlerTypeToCrawler);
    }

    /**
     * Specify the body of the crawler of the specified part of the show being edited.
     *
     * @param crawlerType type of the crawler to specify
     * @param crawler body of the crawler
     * @return new instance of this event with the specified crawler set
     */
    public EditShowCrawlerEvent setCrawler(String crawlerType, String crawler) {
        Map<String, String> updatedCrawlerTypeToCrawler = new HashMap<>(crawlerTypeToCrawler);
        updatedCrawlerTypeToCrawler.put(crawlerType, crawler);
        return new EditShowCrawlerEvent(showId, showName, updatedCrawlerTypeToCrawler);
    }

    /**
     * Check if this event should display a view for editing an existing show or not.
     *
     * @return true if this event is for editing an existing show. Otherwise if this event if for creating a new
     * show - return false.
     */
    public boolean isEditingExistingShow() {
        return showId != null;
    }

    /**
     * Get ID of the show being edited.
     *
     * @return ID of the show being edited. Can be empty if the new show is being created instead.
     */
    public Optional<UUID> getShowId() {
        return Optional.ofNullable(showId);
    }

    /**
     * Get name of the show being edited.
     *
     * @return name of the show being edited. Can be empty if the new show is being created and the show name has
     * not been specified yet.
     */
    public Optional<String> getShowName() {
        return Optional.ofNullable(showName);
    }

    /**
     * Get body of the crawler of the specified type of the edited show.
     *
     * @param crawlerType type of the crawler to return
     * @return body of the specified crawler. Can be empty if the show has the crawler not defined or if the crawler
     * of the specified type does not exist.
     */
    public Optional<String> getCrawler(String crawlerType) {
        return Optional.ofNullable(crawlerTypeToCrawler.get(crawlerType));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getViewId() {
        return ViewIds.EDIT_SHOW_CRAWLER;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EditShowCrawlerEvent event = (EditShowCrawlerEvent) o;
        return Objects.equals(showId, event.showId) &&
                Objects.equals(showName, event.showName) &&
                Objects.equals(crawlerTypeToCrawler, event.crawlerTypeToCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(showId, showName, crawlerTypeToCrawler);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "EditShowCrawlerEvent{" +
                "showId=" + showId +
                ", showName='" + showName + '\'' +
                ", crawlerTypeToCrawler=" + crawlerTypeToCrawler +
                '}';
    }
}
