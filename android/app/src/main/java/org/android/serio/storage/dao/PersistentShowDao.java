package org.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Transaction;

import org.android.serio.storage.model.PersistentEpisode;
import org.android.serio.storage.model.PersistentShow;

import java.util.List;

/**
 * DAO of {@link PersistentShow} and {@link PersistentEpisode}.
 */
@Dao
public abstract class PersistentShowDao {
    /**
     * Find the show with the specified ID.
     *
     * @param id ID of the show
     * @return specified show
     */
    @Query("SELECT * FROM SHOW WHERE ID = :id")
    public abstract PersistentShow findShowById(String id);

    /**
     * Find all shows, ordered by their names.
     *
     * @return all shows
     */
    @Query("SELECT * FROM SHOW ORDER BY NAME ASC")
    public abstract List<PersistentShow> findAllShows();

    /**
     * Count shows, that have the specified name.
     *
     * @param name name to look for
     * @return count of shows with the specified name
     */
    @Query("SELECT COUNT(*) FROM SHOW WHERE NAME = :name")
    public abstract Integer countShowsByName(String name);

    /**
     * Delete the show with the specified ID.
     *
     * @param id ID of the show
     */
    @Query("DELETE FROM SHOW WHERE ID = :id")
    public abstract void deleteShowsById(String id);

    /**
     * Insert the specified show.
     *
     * @param show show to insert
     */
    @Insert
    public abstract void insertShow(PersistentShow show);

    /**
     * Select episodes with the specified show ID, ordered by their (episode's) IDs.
     *
     * @param showId ID of the show
     * @return episodes of the specified show
     */
    @Query("SELECT * FROM EPISODE WHERE SHOW_ID = :showId ORDER BY ID ASC")
    public abstract List<PersistentEpisode> findEpisodesByShowId(String showId);

    /**
     * Insert all the specified episodes.
     *
     * @param episodes episodes to insert
     */
    @Insert
    public abstract void insertAllEpisodes(List<PersistentEpisode> episodes);

    /**
     * Replace the specified show and it's episodes with the new specified show and episodes.
     *
     * <p>All existing episodes of the show will be removed and the new episodes will be
     * inserted instead.</p>
     *
     * @param show show to insert
     * @param episodes episodes to insert
     */
    @Transaction
    public void replaceShowAndItsEpisodes(PersistentShow show, List<PersistentEpisode> episodes) {
        deleteShowsById(show.getId());
        insertShow(show);
        insertAllEpisodes(episodes);
    }
}
