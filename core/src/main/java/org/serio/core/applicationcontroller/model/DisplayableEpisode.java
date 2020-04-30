package org.serio.core.applicationcontroller.model;

import org.serio.core.shows.WatchableEpisode;

import java.util.Objects;

/**
 * An episode that contains all the necessary information to be displayed in the user interface.
 *
 * <p>Serves as an interface adapter between a {@link WatchableEpisode} and the representation, that is
 * understandable by the user interface.</p>
 */
public class DisplayableEpisode {
    private final WatchableEpisode episode;

    /**
     * Construct a displayable episode based off of the specified episode.
     *
     * @param episode actual episode to be based off of. Can't be null.
     */
    public DisplayableEpisode(WatchableEpisode episode) {
        if (episode == null) {
            throw new IllegalArgumentException("Episode is not specified");
        }
        this.episode = episode;
    }

    /**
     * @see WatchableEpisode#getId()
     */
    public int getId() {
        return episode.getId();
    }

    /**
     * @see WatchableEpisode#hasBeenWatched()
     */
    public boolean isWatched() {
        return episode.hasBeenWatched();
    }

    /**
     * @see WatchableEpisode#getName()
     */
    public String getName() {
        return episode.getName();
    }

    /**
     * @see WatchableEpisode#getVideoUrl()
     */
    public String getVideoUrl() {
        return episode.getVideoUrl();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DisplayableEpisode that = (DisplayableEpisode) o;
        return Objects.equals(episode, that.episode);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(episode);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "DisplayableEpisode{" +
                "episode=" + episode +
                '}';
    }
}
