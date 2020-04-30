package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableShow;

import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

/**
 * A TV show that contains all the necessary information to be displayed in the user interface.
 *
 * <p>Serves as an interface adapter between a {@link WatchableShow} and the representation, that is
 * understandable by the user interface.</p>
 *
 * @see DisplayableShowMetaData
 */
public class DisplayableShow {
    private final WatchableShow show;
    private final DisplayableShowMetaData metaData;

    /**
     * Construct a displayable show based off of the specified show.
     *
     * @param show show to be based off of
     * @param dateFormat date format to apply to the show's last watched date
     */
    public DisplayableShow(WatchableShow show, DateFormat dateFormat) {
        this.show = show;
        metaData = new DisplayableShowMetaData(show.getMetaData(), dateFormat);
    }

    /**
     * @see DisplayableShowMetaData#getId()
     */
    public UUID getId() {
        return metaData.getId();
    }

    /**
     * @see DisplayableShowMetaData#getName()
     */
    public String getName() {
        return metaData.getName();
    }

    /**
     * @see DisplayableShowMetaData#getThumbnail()
     */
    public String getThumbnail() {
        return metaData.getThumbnail();
    }

    /**
     * @see DisplayableShowMetaData#getLastWatched()
     */
    public String getLastWatched() {
        return metaData.getLastWatched();
    }

    /**
     * @see WatchableShow#getEpisodes()
     */
    public List<DisplayableEpisode> getEpisodes() {
        return show.getEpisodes()
                .stream()
                .map(DisplayableEpisode::new)
                .collect(Collectors.toList());
    }

    /**
     * @see WatchableShow#getEpisodeById(int)
     */
    public Optional<DisplayableEpisode> getEpisodeById(int episodeId) {
        return show.getEpisodeById(episodeId).map(DisplayableEpisode::new);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableShow show1 = (DisplayableShow) o;
        return Objects.equals(show, show1.show) &&
                Objects.equals(metaData, show1.metaData);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(show, metaData);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "DisplayableShow{" +
                "show=" + show +
                ", metaData=" + metaData +
                '}';
    }
}
