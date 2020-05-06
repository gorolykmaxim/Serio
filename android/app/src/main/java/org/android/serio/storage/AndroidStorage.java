package org.android.serio.storage;

import android.content.Context;

import androidx.room.Room;

import org.android.serio.storage.dao.PersistentCrawlLogEntryDao;
import org.android.serio.storage.dao.PersistentEpisodeViewDao;
import org.android.serio.storage.dao.PersistentShowCrawlerDao;
import org.android.serio.storage.dao.PersistentShowDao;
import org.android.serio.storage.model.PersistentCrawlLogEntry;
import org.android.serio.storage.model.PersistentEpisode;
import org.android.serio.storage.model.PersistentEpisodeView;
import org.android.serio.storage.model.PersistentShow;
import org.android.serio.storage.model.PersistentShowCrawler;
import org.android.serio.storage.model.PersistentShowView;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showcrawlerlogstorage.ShowCrawlerLogStorage;
import org.serio.core.showcrawlerstorage.ShowCrawlerStorage;
import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowMetaData;
import org.serio.core.showstorage.ShowStorage;
import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.ShowView;
import org.serio.core.watchhistory.WatchHistory;
import org.serio.core.watchhistory.WatchProgress;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

/**
 * Android implementation of all the storage modules of the {@link org.serio.core.Core}.
 *
 * <p>Uses the Room ORM to store data in a SQLite database.</p>
 */
public class AndroidStorage implements ShowCrawlerLogStorage, ShowCrawlerStorage, ShowStorage, WatchHistory {
    private PersistentCrawlLogEntryDao crawlLogEntryDao;
    private PersistentEpisodeViewDao episodeViewDao;
    private PersistentShowCrawlerDao showCrawlerDao;
    private PersistentShowDao showDao;

    /**
     * Construct a storage.
     *
     * @param context context that will be used to build a database instance
     */
    public AndroidStorage(Context context) {
        SerioDatabase database = Room.databaseBuilder(context, SerioDatabase.class, SerioDatabase.NAME)
                .build();
        crawlLogEntryDao = database.getCrawlLogEntryDao();
        episodeViewDao = database.getEpisodeViewDao();
        showCrawlerDao = database.getShowCrawlerDao();
        showDao = database.getShowDao();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<Optional<List<CrawlLogEntry>>> getLastCrawlingLogOfTheShow(String showId) {
        return execute(() -> {
            List<CrawlLogEntry> entries = crawlLogEntryDao.findByShowId(showId)
                    .stream()
                    .map(PersistentCrawlLogEntry::toCrawlLogEntry)
                    .collect(Collectors.toList());
            return entries.isEmpty() ? Optional.empty() : Optional.of(entries);
        }, "get last crawling log of the show with ID '%s'", showId);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void saveCrawlingLogOfTheShow(String showId, List<CrawlLogEntry> log) {
       execute(() -> {
           List<PersistentCrawlLogEntry> persistentCrawlLogEntries = new ArrayList<>();
           for (int i = 0; i < log.size(); i++) {
               CrawlLogEntry entry = log.get(i);
               persistentCrawlLogEntries.add(new PersistentCrawlLogEntry(i, showId, entry));
           }
           crawlLogEntryDao.replaceLastCrawlLogOfTheShow(showId, persistentCrawlLogEntries);
       }, "save crawling log of the show with ID '%s': %s", showId, log);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<String> findShowCrawlerByShowId(String showId) {
        return execute(
                () -> showCrawlerDao.findByShowId(showId).toCrawler(),
                "find crawler of the show with ID '%s'", showId
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void saveShowCrawler(String showId, String showCrawler) {
        execute(
                () -> showCrawlerDao.insert(new PersistentShowCrawler(showId, showCrawler)),
                "save crawler of the show with ID '%s': %s", showId, showCrawler
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void deleteShowCrawlerByShowId(String showId) {
        execute(
                () -> showCrawlerDao.deleteByShowId(showId),
                "delete crawler of the show with ID '%s", showId
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<Show> findById(UUID id) {
        return execute(() -> {
            String showId = id.toString();
            ShowMetaData metaData = showDao.findShowById(showId).toShowMetaData();
            List<Episode> episodes = showDao.findEpisodesByShowId(showId)
                    .stream()
                    .map(PersistentEpisode::toEpisode)
                    .collect(Collectors.toList());
            return new Show(metaData, episodes);
        }, "find show with ID '%s'", id);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<List<ShowMetaData>> findAll() {
        return execute(
                () -> showDao.findAllShows().stream().map(PersistentShow::toShowMetaData).collect(Collectors.toList()),
                "find all shows"
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<Boolean> containsShowWithName(String name) {
        return execute(
                () -> showDao.countShowsByName(name) > 0,
                "check if a show with name '%s' exists", name
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void save(Show show) {
        execute(() -> {
            PersistentShow persistentShow = new PersistentShow(show.getMetaData());
            List<PersistentEpisode> persistentEpisodes = show.getEpisodes()
                    .stream()
                    .map(episode -> new PersistentEpisode(show, episode))
                    .collect(Collectors.toList());
            showDao.replaceShowAndItsEpisodes(persistentShow, persistentEpisodes);
        }, "save %s", show);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void deleteById(UUID id) {
        execute(
                () -> showDao.deleteShowsById(id.toString()),
                "delete show with ID '%s'", id
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<List<ShowView>> getShowWatchHistory() {
        return execute(() ->
            episodeViewDao.findShowViews()
                    .stream()
                    .map(PersistentShowView::toShowView)
                    .collect(Collectors.toList()),
                "get watch history of all shows");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Future<List<EpisodeView>> getEpisodeWatchHistoryOfShow(String showId) {
        return execute(() ->
                episodeViewDao.findByShowId(showId)
                        .stream()
                        .map(PersistentEpisodeView::toEpisodeView)
                        .collect(Collectors.toList()),
                "get watch history of the show with ID '%s'", showId
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void watchShowEpisode(String showId, String episodeId, WatchProgress watchProgress) {
        execute(
                () -> episodeViewDao.insert(new PersistentEpisodeView(showId, episodeId, watchProgress)),
                "watch episode '%s' of the show with ID '%s' for %s", episodeId, showId, watchProgress
        );
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearWatchHistoryOfShow(String showId) {
        execute(
                () -> episodeViewDao.deleteByShowId(showId),
                "clear watch history of the show with ID '%s'", showId
        );
    }

    private <T> Future<T> execute(Callable<T> callable, String intentTemplate, Object ...arguments) {
        try {
            return CompletableFuture.completedFuture(callable.call());
        } catch (Exception e) {
            throw wrapIntoStorageException(e, intentTemplate, arguments);
        }
    }

    private void execute(Runnable runnable, String intentTemplate, Object ...arguments) {
        try {
            runnable.run();
        } catch (Exception e) {
            throw wrapIntoStorageException(e, intentTemplate, arguments);
        }
    }

    private StorageException wrapIntoStorageException(Throwable e, String intentTemplate, Object ...arguments) {
        return new StorageException(String.format(intentTemplate, arguments), e);
    }
}
