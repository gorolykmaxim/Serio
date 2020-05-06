package org.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;

import org.android.serio.storage.model.PersistentEpisodeView;
import org.android.serio.storage.model.PersistentShowView;

import java.util.List;

/**
 * DAO of {@link PersistentEpisodeView} and {@link PersistentShowView}.
 */
@Dao
public interface PersistentEpisodeViewDao {
    /**
     * Find list of all shows, that have been watched, and their last watch dates.
     *
     * @return list of all show views
     */
    @Query("SELECT SHOW_ID, MAX(LAST_WATCH_DATE) AS LAST_WATCH_DATE FROM EPISODE_VIEW GROUP BY SHOW_ID")
    List<PersistentShowView> findShowViews();

    /**
     * Find list of all episode views with the specified show ID.
     *
     * @param showId ID of the show
     * @return list of all episode views
     */
    @Query("SELECT * FROM EPISODE_VIEW WHERE SHOW_ID = :showId")
    List<PersistentEpisodeView> findByShowId(String showId);

    /**
     * Insert the specified episode view. If the view already exists - replace it.
     *
     * @param episodeView episode view to insert
     */
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    void insert(PersistentEpisodeView episodeView);

    /**
     * Delete all episode views with the specified show ID.
     *
     * @param showId ID of the show
     */
    @Query("DELETE FROM EPISODE_VIEW WHERE SHOW_ID = :showId")
    void deleteByShowId(String showId);
}
