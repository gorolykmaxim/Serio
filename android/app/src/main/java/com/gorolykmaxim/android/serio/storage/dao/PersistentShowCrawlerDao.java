package com.gorolykmaxim.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;

import com.gorolykmaxim.android.serio.storage.model.PersistentShowCrawler;

@Dao
public interface PersistentShowCrawlerDao {
    @Query("SELECT * FROM SHOW_CRAWLER WHERE SHOW_ID = :showId")
    PersistentShowCrawler findByShowId(String showId);
    @Query("DELETE FROM SHOW_CRAWLER WHERE SHOW_ID = :showId")
    void deleteByShowId(String showId);
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    void insert(PersistentShowCrawler persistentShowCrawler);
}
