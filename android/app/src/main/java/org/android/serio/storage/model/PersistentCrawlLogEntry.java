package org.android.serio.storage.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;

/**
 * Database representation of {@link CrawlLogEntry}.
 */
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

    /**
     * This constructor is used by the Room.
     */
    public PersistentCrawlLogEntry(int id, @NonNull String showId, @NonNull String value, String input, String output) {
        this.id = id;
        this.showId = showId;
        this.value = value;
        this.input = input;
        this.output = output;
    }

    /**
     * Construct a persistent version of the specified crawl log entry.
     *
     * @param id position of the specified entry in it's log
     * @param showId ID of the show, crawling of which has produced this log
     * @param crawlLogEntry log entry to be constructed based of
     */
    public PersistentCrawlLogEntry(int id, @NonNull String showId, CrawlLogEntry crawlLogEntry) {
        this.id = id;
        this.showId = showId;
        this.value = crawlLogEntry.getValue();
        this.input = crawlLogEntry.getInputInformation().orElse(null);
        this.output = crawlLogEntry.getOutputInformation().orElse(null);
    }

    /**
     * Get position of the entry in it's log.
     *
     * @return position of the entry in it's log.
     */
    public int getId() {
        return id;
    }

    /**
     * Get ID of the show, crawling of which has produced this log.
     *
     * @return ID of the show
     */
    @NonNull
    public String getShowId() {
        return showId;
    }

    /**
     * @see CrawlLogEntry#getValue()
     */
    @NonNull
    public String getValue() {
        return value;
    }

    /**
     * @see CrawlLogEntry#getInputInformation()
     */
    public String getInput() {
        return input;
    }

    /**
     * @see CrawlLogEntry#getOutputInformation()
     */
    public String getOutput() {
        return output;
    }

    /**
     * Convert this entry into it's original version.
     *
     * @return original {@link CrawlLogEntry}
     */
    public CrawlLogEntry toCrawlLogEntry() {
        return new CrawlLogEntry(value, input, output);
    }
}
