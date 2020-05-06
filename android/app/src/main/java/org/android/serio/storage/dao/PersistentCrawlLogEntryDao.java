package org.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Transaction;

import org.android.serio.storage.model.PersistentCrawlLogEntry;

import java.util.List;

/**
 * DAO of {@link PersistentCrawlLogEntry}.
 */
@Dao
public abstract class PersistentCrawlLogEntryDao {
    /**
     * Find all log entries with the specified show ID.
     *
     * @param showId ID of the show
     * @return list of log entries
     */
    @Query("SELECT * FROM CRAWL_LOG_ENTRY WHERE SHOW_ID = :showId ORDER BY ID ASC")
    public abstract List<PersistentCrawlLogEntry> findByShowId(String showId);

    /**
     * Delete all log entries with the specified show ID.
     *
     * @param showId ID of the show
     */
    @Query("DELETE FROM CRAWL_LOG_ENTRY WHERE SHOW_ID = :showId")
    public abstract void deleteById(String showId);

    /**
     * Insert all the specified log entries.
     *
     * @param entries log entries to insert
     */
    @Insert
    public abstract void insertAll(List<PersistentCrawlLogEntry> entries);

    /**
     * Replace all existing log entries with the specified show ID with the specified new ones.
     *
     * @param showId ID of the show
     * @param newLog new log entries to insert
     */
    @Transaction
    public void replaceLastCrawlLogOfTheShow(String showId, List<PersistentCrawlLogEntry> newLog) {
        deleteById(showId);
        insertAll(newLog);
    }
}
