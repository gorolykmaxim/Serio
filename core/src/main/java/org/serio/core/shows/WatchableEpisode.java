package org.serio.core.shows;

import org.serio.core.showstorage.Episode;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.WatchProgress;

import java.time.LocalDateTime;
import java.util.Objects;
import java.util.Optional;

/**
 * An episode of a show that a user might have watched or might be watching.
 *
 * @see Episode
 */
public class WatchableEpisode {
    private final Episode episode;
    private final EpisodeView view;

    /**
     * Construct a watchable episode.
     *
     * @param episode an actual episode
     * @param view view of this episode. Must be null if a user have not watched this episode at all.
     */
    public WatchableEpisode(Episode episode, EpisodeView view) {
        if (episode == null) {
            throw new IllegalArgumentException("Episode is not specified");
        }
        this.episode = episode;
        this.view = view;
    }

    /**
     * @see Episode#getId()
     */
    public int getId() {
        return episode.getId();
    }

    /**
     * @see Episode#getName()
     */
    public String getName() {
        return episode.getName();
    }

    /**
     * @see Episode#getVideoUrl()
     */
    public String getVideoUrl() {
        return episode.getVideoUrl();
    }

    /**
     * Get duration of this episode the user has already seen.
     *
     * @see EpisodeView#getWatchProgress()
     *
     * @return watched duration of this episode. If the user haven't seen this episode at all - {@link WatchProgress#NONE}
     * will be returned.
     */
    public WatchProgress getWatchProgress() {
        return view != null ? view.getWatchProgress() : WatchProgress.NONE;
    }

    /**
     * @see EpisodeView#isComplete()
     */
    public boolean hasBeenWatchedCompletely() {
        return view != null && view.isComplete();
    }

    /**
     * Check if this episode has been watched by the user.
     *
     * @return true if the episode has been watched by the user
     */
    public boolean hasBeenWatched() {
        return view != null;
    }

    /**
     * Get the date when the user has watched this episode the last time.
     *
     * @see EpisodeView#getLastWatchDate()
     *
     * @return optional of the last episode watch date. If the user hasn't watched this episode - an empty optional
     * will be returned.
     */
    public Optional<LocalDateTime> getLastWatchDate() {
        return Optional.ofNullable(view != null ? view.getLastWatchDate() : null);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        WatchableEpisode that = (WatchableEpisode) o;
        return Objects.equals(episode, that.episode) &&
                Objects.equals(view, that.view);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(episode, view);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "WatchableEpisode{" +
                "episode=" + episode +
                ", view=" + view +
                '}';
    }
}