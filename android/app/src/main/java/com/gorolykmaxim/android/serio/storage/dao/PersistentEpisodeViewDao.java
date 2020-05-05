package com.gorolykmaxim.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;

import com.gorolykmaxim.android.serio.storage.model.PersistentEpisodeView;
import com.gorolykmaxim.android.serio.storage.model.PersistentShowView;

import java.util.List;

@Dao
public interface PersistentEpisodeViewDao {
    @Query("SELECT SHOW_ID, MAX(LAST_WATCH_DATE) AS LAST_WATCH_DATE FROM EPISODE_VIEW GROUP BY SHOW_ID")
    List<PersistentShowView> findShowViews();
    @Query("SELECT * FROM EPISODE_VIEW WHERE SHOW_ID = :showId")
    List<PersistentEpisodeView> findByShowId(String showId);
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    void insert(PersistentEpisodeView episodeView);
    @Query("DELETE FROM EPISODE_VIEW WHERE SHOW_ID = :showId")
    void deleteByShowId(String showId);
}
