package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

/**
 * Database representation of a show crawler.
 */
@Entity(tableName = "SHOW_CRAWLER", primaryKeys = {"SHOW_ID"})
public class PersistentShowCrawler {
    @ColumnInfo(name = "SHOW_ID")
    @NonNull
    private String showId;
    @ColumnInfo(name = "CRAWLER")
    @NonNull
    private String crawler;

    /**
     * Construct a show crawler.
     *
     * @param showId ID of the show, being crawled by this crawler
     * @param crawler show crawler body
     */
    public PersistentShowCrawler(@NonNull String showId, @NonNull String crawler) {
        this.showId = showId;
        this.crawler = crawler;
    }

    /**
     * Get ID of the show, being crawled by this crawler.
     *
     * @return ID of the show
     */
    @NonNull
    public String getShowId() {
        return showId;
    }

    /**
     * Get body of this show crawler.
     *
     * @return body of the show crawler
     */
    @NonNull
    public String getCrawler() {
        return crawler;
    }

    /**
     * @see PersistentShowCrawler#getCrawler()
     */
    public String toCrawler() {
        return crawler;
    }
}
