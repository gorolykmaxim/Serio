package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.WatchProgress;

import java.time.LocalDateTime;

/**
 * Database representation of {@link EpisodeView}.
 */
@Entity(tableName = "EPISODE_VIEW", primaryKeys = {"SHOW_ID", "EPISODE_ID"})
public class PersistentEpisodeView {
    @ColumnInfo(name = "SHOW_ID")
    @NonNull
    private String showId;
    @ColumnInfo(name = "EPISODE_ID")
    @NonNull
    private String episodeId;
    @ColumnInfo(name = "LAST_WATCH_DATE")
    private long lastWatchDate;
    @ColumnInfo(name = "PROGRESS")
    private double progress;

    /**
     * This constructor is used by the Room.
     */
    public PersistentEpisodeView(@NonNull String showId, @NonNull String episodeId, long lastWatchDate, double progress) {
        this.showId = showId;
        this.episodeId = episodeId;
        this.lastWatchDate = lastWatchDate;
        this.progress = progress;
    }

    /**
     * Construct a persistent version of a corresponding {@link EpisodeView}.
     *
     * @param showId ID of the show, that has been watched
     * @param episodeId ID of the episode, that has been watched
     * @param progress current watch progress of the episode of the show
     */
    public PersistentEpisodeView(@NonNull String showId, @NonNull String episodeId, WatchProgress progress) {
        this.showId = showId;
        this.episodeId = episodeId;
        this.lastWatchDate = PersistentLocalDateTime.from(LocalDateTime.now()).toMilliseconds();
        this.progress = progress.getPercentage();
    }

    /**
     * Get ID of the show, that has been watched.
     *
     * @return ID of the show
     */
    @NonNull
    public String getShowId() {
        return showId;
    }

    /**
     * Get ID of the episode, that has been watched.
     *
     * @return ID of the episode
     */
    @NonNull
    public String getEpisodeId() {
        return episodeId;
    }

    /**
     * Get the date when this view updated the last time (when the corresponding episode of the
     * show has been watched for the last time).
     *
     * @see EpisodeView#getLastWatchDate()
     *
     * @return last watched date in milliseconds since epoch
     */
    public long getLastWatchDate() {
        return lastWatchDate;
    }

    /**
     * Get current watch progress of the episode.
     *
     * @see WatchProgress#getPercentage()
     *
     * @return current watch progress
     */
    public double getProgress() {
        return progress;
    }

    /**
     * Convert this view into it's original version.
     *
     * @return original {@link EpisodeView}
     */
    public EpisodeView toEpisodeView() {
        return new EpisodeView(
                episodeId,
                WatchProgress.fromPercentage(progress),
                PersistentLocalDateTime.from(lastWatchDate).toLocalDateTime()
        );
    }
}
