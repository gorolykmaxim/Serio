package com.gorolykmaxim.android.serio.storage.dao;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Transaction;

import com.gorolykmaxim.android.serio.storage.model.PersistentEpisode;
import com.gorolykmaxim.android.serio.storage.model.PersistentShow;

import java.util.List;

@Dao
public abstract class PersistentShowDao {
    @Query("SELECT * FROM SHOW WHERE ID = :id")
    public abstract PersistentShow findShowById(String id);
    @Query("SELECT * FROM SHOW ORDER BY NAME ASC")
    public  abstract List<PersistentShow> findAllShows();
    @Query("SELECT COUNT(*) FROM SHOW WHERE NAME = :name")
    public abstract Integer countShowsByName(String name);
    @Query("DELETE FROM SHOW WHERE ID = :id")
    public abstract void deleteShowsById(String id);
    @Insert
    public abstract void insertShow(PersistentShow show);
    @Query("SELECT * FROM EPISODE WHERE SHOW_ID = :showId ORDER BY ID ASC")
    public abstract List<PersistentEpisode> findEpisodesByShowId(String showId);
    @Insert
    public abstract void insertAllEpisodes(List<PersistentEpisode> episodes);
    @Transaction
    public void replaceShowAndItsEpisodes(PersistentShow show, List<PersistentEpisode> episodes) {
        deleteShowsById(show.getId());
        insertShow(show);
        insertAllEpisodes(episodes);
    }
}
