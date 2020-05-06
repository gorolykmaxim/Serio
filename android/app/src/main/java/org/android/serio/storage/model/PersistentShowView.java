package org.android.serio.storage.model;

import androidx.room.ColumnInfo;

import org.serio.core.watchhistory.ShowView;

public class PersistentShowView {
    @ColumnInfo(name = "SHOW_ID")
    public String showId;
    @ColumnInfo(name = "LAST_WATCH_DATE")
    private long lastWatchedDate;

    public PersistentShowView(String showId, long lastWatchedDate) {
        this.showId = showId;
        this.lastWatchedDate = lastWatchedDate;
    }

    public String getShowId() {
        return showId;
    }

    public long getLastWatchedDate() {
        return lastWatchedDate;
    }

    public ShowView toShowView() {
        return new ShowView(
                showId,
                PersistentLocalDateTime.from(lastWatchedDate).toLocalDateTime()
        );
    }
}
