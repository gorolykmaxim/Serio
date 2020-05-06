package org.android.serio.storage.model;

import androidx.room.ColumnInfo;

import org.serio.core.watchhistory.ShowView;

/**
 * Database representation of {@link ShowView}.
 */
public class PersistentShowView {
    @ColumnInfo(name = "SHOW_ID")
    public String showId;
    @ColumnInfo(name = "LAST_WATCH_DATE")
    private long lastWatchedDate;

    /**
     * This constructor is used by the Room.
     */
    public PersistentShowView(String showId, long lastWatchedDate) {
        this.showId = showId;
        this.lastWatchedDate = lastWatchedDate;
    }

    /**
     * @see PersistentEpisodeView#getShowId()
     */
    public String getShowId() {
        return showId;
    }

    /**
     * Get the most recent date when one of the episodes of this show has been watched.
     *
     * @see PersistentEpisodeView#getLastWatchDate()
     */
    public long getLastWatchedDate() {
        return lastWatchedDate;
    }

    /**
     * Convert this show view into it's original version.
     *
     * @return original {@link ShowView}
     */
    public ShowView toShowView() {
        return new ShowView(
                showId,
                PersistentLocalDateTime.from(lastWatchedDate).toLocalDateTime()
        );
    }
}
