package com.gorolykmaxim.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Transaction;

import com.gorolykmaxim.android.serio.storage.model.PersistentCrawlLogEntry;

import java.util.List;

@Dao
public abstract class PersistentCrawlLogEntryDao {
    @Query("SELECT * FROM CRAWL_LOG_ENTRY WHERE SHOW_ID = :showId ORDER BY ID ASC")
    public abstract List<PersistentCrawlLogEntry> findByShowId(String showId);
    @Query("DELETE FROM CRAWL_LOG_ENTRY WHERE SHOW_ID = :showId")
    public abstract void deleteById(String showId);
    @Insert
    public abstract void insertAll(List<PersistentCrawlLogEntry> entries);

    @Transaction
    public void replaceLastCrawlLogOfTheShow(String showId, List<PersistentCrawlLogEntry> newLog) {
        deleteById(showId);
        insertAll(newLog);
    }
}
