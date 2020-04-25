package org.serio.core.watchhistory;

import java.time.LocalDateTime;
import java.util.Objects;

/**
 * User's view of an episode.
 *
 * <p>Indicates that the user has either watched or is watching the corresponding episode of a show.</p>
 */
public class EpisodeView {
    private final String episodeId;
    private final WatchProgress watchProgress;
    private final LocalDateTime lastWatchDate;

    /**
     * @see EpisodeView#EpisodeView(String, WatchProgress, LocalDateTime)
     */
    public EpisodeView(String episodeId) {
        this(episodeId, null, null);
    }

    /**
     * @see EpisodeView#EpisodeView(String, WatchProgress, LocalDateTime)
     */
    public EpisodeView(String episodeId, WatchProgress watchProgress) {
        this(episodeId, watchProgress, null);
    }

    /**
     * Construct a view of an episode.
     *
     * @param episodeId ID of the episode, that has been viewed
     * @param watchProgress amount of the episode's duration, that has been watched by the user so far
     * @param lastWatchDate the date of the last time the user has watched the specified episode.
     *                      If not specified - today's date will be used.
     */
    public EpisodeView(String episodeId, WatchProgress watchProgress, LocalDateTime lastWatchDate) {
        if (episodeId == null) {
            throw new IllegalArgumentException("Episode ID is not specified");
        }
        this.episodeId = episodeId;
        this.watchProgress = watchProgress != null ? watchProgress : WatchProgress.NONE;
        this.lastWatchDate = lastWatchDate != null ? lastWatchDate : LocalDateTime.now();
    }

    /**
     * Get ID of the episode, that has been watched.
     *
     * @return ID of the episode
     */
    public String getEpisodeId() {
        return episodeId;
    }

    /**
     * Get amount of episode's duration, that has been watched by the user so far.
     *
     * @return duration of the episode the user has already seen
     */
    public WatchProgress getWatchProgress() {
        return watchProgress;
    }

    /**
     * Check if the user has watched this episode from start to finish
     *
     * @return true if the user has completely watched this episode
     */
    public boolean isComplete() {
        return WatchProgress.COMPLETE.equals(watchProgress);
    }

    /**
     * Get the date when this view was created.
     *
     * @return date of the last time the corresponding episode has been watched
     */
    public LocalDateTime getLastWatchDate() {
        return lastWatchDate;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EpisodeView that = (EpisodeView) o;
        return Objects.equals(episodeId, that.episodeId) &&
                Objects.equals(watchProgress, that.watchProgress) &&
                Objects.equals(lastWatchDate, that.lastWatchDate);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int hashCode() {
        return Objects.hash(episodeId, watchProgress, lastWatchDate);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        return "EpisodeView{" +
                "episodeId='" + episodeId + '\'' +
                ", watchProgress=" + watchProgress +
                ", lastWatchDate=" + lastWatchDate +
                '}';
    }
}