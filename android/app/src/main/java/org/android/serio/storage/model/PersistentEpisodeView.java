package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.WatchProgress;

import java.time.LocalDateTime;

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

    public PersistentEpisodeView(@NonNull String showId, @NonNull String episodeId, long lastWatchDate, double progress) {
        this.showId = showId;
        this.episodeId = episodeId;
        this.lastWatchDate = lastWatchDate;
        this.progress = progress;
    }

    public PersistentEpisodeView(@NonNull String showId, @NonNull String episodeId, WatchProgress progress) {
        this.showId = showId;
        this.episodeId = episodeId;
        this.lastWatchDate = PersistentLocalDateTime.from(LocalDateTime.now()).toMilliseconds();
        this.progress = progress.getPercentage();
    }

    @NonNull
    public String getShowId() {
        return showId;
    }

    @NonNull
    public String getEpisodeId() {
        return episodeId;
    }

    public long getLastWatchDate() {
        return lastWatchDate;
    }

    public double getProgress() {
        return progress;
    }

    public EpisodeView toEpisodeView() {
        return new EpisodeView(
                episodeId,
                WatchProgress.fromPercentage(progress),
                PersistentLocalDateTime.from(lastWatchDate).toLocalDateTime()
        );
    }
}
