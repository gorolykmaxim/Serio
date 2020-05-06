package org.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;

import org.android.serio.storage.model.PersistentShowCrawler;

/**
 * DAO of {@link PersistentShowCrawler}.
 */
@Dao
public interface PersistentShowCrawlerDao {
    /**
     * Find the show crawler with the specified show ID.
     *
     * @param showId ID of the show
     * @return show crawler
     */
    @Query("SELECT * FROM SHOW_CRAWLER WHERE SHOW_ID = :showId")
    PersistentShowCrawler findByShowId(String showId);

    /**
     * Delete the show crawler with the specified show ID.
     *
     * @param showId ID of the show
     */
    @Query("DELETE FROM SHOW_CRAWLER WHERE SHOW_ID = :showId")
    void deleteByShowId(String showId);

    /**
     * Insert the specified show crawler. If a crawler with the same show ID already exists -
     * replace it with the specified one.
     *
     * @param persistentShowCrawler show crawler to insert
     */
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    void insert(PersistentShowCrawler persistentShowCrawler);
}
