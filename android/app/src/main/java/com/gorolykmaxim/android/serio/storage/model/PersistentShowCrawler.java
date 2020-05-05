package com.gorolykmaxim.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

@Entity(tableName = "SHOW_CRAWLER", primaryKeys = {"SHOW_ID"})
public class PersistentShowCrawler {
    @ColumnInfo(name = "SHOW_ID")
    @NonNull
    private String showId;
    @ColumnInfo(name = "CRAWLER")
    @NonNull
    private String crawler;


    public PersistentShowCrawler(@NonNull String showId, @NonNull String crawler) {
        this.showId = showId;
        this.crawler = crawler;
    }

    @NonNull
    public String getShowId() {
        return showId;
    }

    @NonNull
    public String getCrawler() {
        return crawler;
    }

    public String toCrawler() {
        return crawler;
    }
}
