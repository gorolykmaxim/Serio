package com.gorolykmaxim.android.serio.storage;

import androidx.room.Database;
import androidx.room.RoomDatabase;

import com.gorolykmaxim.android.serio.storage.dao.PersistentCrawlLogEntryDao;
import com.gorolykmaxim.android.serio.storage.dao.PersistentEpisodeViewDao;
import com.gorolykmaxim.android.serio.storage.dao.PersistentShowCrawlerDao;
import com.gorolykmaxim.android.serio.storage.dao.PersistentShowDao;
import com.gorolykmaxim.android.serio.storage.model.PersistentCrawlLogEntry;
import com.gorolykmaxim.android.serio.storage.model.PersistentEpisode;
import com.gorolykmaxim.android.serio.storage.model.PersistentEpisodeView;
import com.gorolykmaxim.android.serio.storage.model.PersistentShow;
import com.gorolykmaxim.android.serio.storage.model.PersistentShowCrawler;

@Database(entities = {PersistentCrawlLogEntry.class, PersistentEpisode.class, PersistentEpisodeView.class, PersistentShow.class, PersistentShowCrawler.class}, version = 1)
public abstract class SerioDatabase extends RoomDatabase {
    public static final String NAME = "serio";

    public abstract PersistentCrawlLogEntryDao getCrawlLogEntryDao();
    public abstract PersistentEpisodeViewDao getEpisodeViewDao();
    public abstract PersistentShowCrawlerDao getShowCrawlerDao();
    public abstract PersistentShowDao getShowDao();
}
