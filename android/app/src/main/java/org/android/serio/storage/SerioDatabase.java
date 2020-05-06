package org.android.serio.storage;

import androidx.room.Database;
import androidx.room.RoomDatabase;

import org.android.serio.storage.dao.PersistentCrawlLogEntryDao;
import org.android.serio.storage.dao.PersistentEpisodeViewDao;
import org.android.serio.storage.dao.PersistentShowCrawlerDao;
import org.android.serio.storage.dao.PersistentShowDao;
import org.android.serio.storage.model.PersistentCrawlLogEntry;
import org.android.serio.storage.model.PersistentEpisode;
import org.android.serio.storage.model.PersistentEpisodeView;
import org.android.serio.storage.model.PersistentShow;
import org.android.serio.storage.model.PersistentShowCrawler;

@Database(entities = {PersistentCrawlLogEntry.class, PersistentEpisode.class, PersistentEpisodeView.class, PersistentShow.class, PersistentShowCrawler.class}, version = 1)
public abstract class SerioDatabase extends RoomDatabase {
    public static final String NAME = "serio";

    public abstract PersistentCrawlLogEntryDao getCrawlLogEntryDao();
    public abstract PersistentEpisodeViewDao getEpisodeViewDao();
    public abstract PersistentShowCrawlerDao getShowCrawlerDao();
    public abstract PersistentShowDao getShowDao();
}
