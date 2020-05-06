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

/**
 * Android Serio database.
 */
@Database(entities = {PersistentCrawlLogEntry.class, PersistentEpisode.class, PersistentEpisodeView.class, PersistentShow.class, PersistentShowCrawler.class}, version = 1)
public abstract class SerioDatabase extends RoomDatabase {
    /**
     * Name of the database.
     */
    public static final String NAME = "serio";

    /**
     * Get DAO of {@link PersistentCrawlLogEntry}.
     *
     * @return DAO of crawl log entries
     */
    public abstract PersistentCrawlLogEntryDao getCrawlLogEntryDao();

    /**
     * Get DAO of {@link PersistentEpisodeView}.
     *
     * @return DAO of episode view
     */
    public abstract PersistentEpisodeViewDao getEpisodeViewDao();

    /**
     * Get DAO of {@link PersistentShowCrawler}.
     *
     * @return DAO of show crawlers
     */
    public abstract PersistentShowCrawlerDao getShowCrawlerDao();

    /**
     * Get DAO of {@link PersistentShow}.
     *
     * @return DAO of shows
     */
    public abstract PersistentShowDao getShowDao();
}
