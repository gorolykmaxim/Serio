package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;

@Entity(tableName = "CRAWL_LOG_ENTRY", primaryKeys = {"ID", "SHOW_ID"})
public class PersistentCrawlLogEntry {
    @ColumnInfo(name = "ID")
    private int id;
    @ColumnInfo(name = "SHOW_ID")
    @NonNull
    private String showId;
    @ColumnInfo(name = "VALUE")
    @NonNull
    private String value;
    @ColumnInfo(name = "INPUT")
    private String input;
    @ColumnInfo(name = "OUTPUT")
    private String output;

    public PersistentCrawlLogEntry(int id, @NonNull String showId, @NonNull String value, String input, String output) {
        this.id = id;
        this.showId = showId;
        this.value = value;
        this.input = input;
        this.output = output;
    }

    public PersistentCrawlLogEntry(int id, @NonNull String showId, CrawlLogEntry crawlLogEntry) {
        this.id = id;
        this.showId = showId;
        this.value = crawlLogEntry.getValue();
        this.input = crawlLogEntry.getInputInformation().orElse(null);
        this.output = crawlLogEntry.getOutputInformation().orElse(null);
    }

    public int getId() {
        return id;
    }

    @NonNull
    public String getShowId() {
        return showId;
    }

    @NonNull
    public String getValue() {
        return value;
    }

    public String getInput() {
        return input;
    }

    public String getOutput() {
        return output;
    }

    public CrawlLogEntry toCrawlLogEntry() {
        return new CrawlLogEntry(value, input, output);
    }
}
