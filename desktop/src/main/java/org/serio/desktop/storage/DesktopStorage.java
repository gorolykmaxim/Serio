package org.serio.desktop.storage;

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
import org.serio.desktop.storage.show.*;
import org.serio.desktop.storage.show.crawler.DeleteShowCrawlerByShowId;
import org.serio.desktop.storage.show.crawler.FindShowCrawlerByShowIdTask;
import org.serio.desktop.storage.show.crawler.SaveShowCrawlerTask;
import org.serio.desktop.storage.show.crawler.log.CrawlLogEntryRowMapper;
import org.serio.desktop.storage.show.crawler.log.GetLastCrawlingLogOfTheShowTask;
import org.serio.desktop.storage.show.crawler.log.SaveCrawlingLogOfTheShowTask;
import org.serio.desktop.storage.watchhistory.*;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;

import javax.sql.DataSource;
import java.util.List;
import java.util.Optional;
import java.util.Properties;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;

public class DesktopStorage implements WatchHistory, ShowStorage, ShowCrawlerStorage, ShowCrawlerLogStorage {
    private final JdbcTemplate template;
    private final PlatformTransactionManager transactionManager;
    private final RowMapper<CrawlLogEntry> crawlLogEntryRowMapper;
    private final RowMapper<ShowMetaData> showMetaDataRowMapper;
    private final RowMapper<Episode> episodeRowMapper;
    private final RowMapper<ShowView> showViewRowMapper;
    private final RowMapper<EpisodeView> episodeViewRowMapper;
    private final Properties queries;
    private final String schemaInitializationScript;

    public DesktopStorage(DataSource dataSource, Properties queries, String schemaInitializationScript) {
        template = new JdbcTemplate(dataSource);
        transactionManager = new DataSourceTransactionManager(dataSource);
        this.queries = queries;
        this.schemaInitializationScript = schemaInitializationScript;
        crawlLogEntryRowMapper = new CrawlLogEntryRowMapper();
        showMetaDataRowMapper = new ShowMetaDataRowMapper();
        episodeRowMapper = new EpisodeRowMapper();
        showViewRowMapper = new ShowViewRowMapper();
        episodeViewRowMapper = new EpisodeViewRowMapper();
    }

    public void initialize() {
        execute(
                new InitializeStorageTask(schemaInitializationScript),
                true,
                "Initialize database schema: %s", schemaInitializationScript
        );
    }

    @Override
    public Future<Optional<List<CrawlLogEntry>>> getLastCrawlingLogOfTheShow(String showId) {
        return execute(
                new GetLastCrawlingLogOfTheShowTask(showId, crawlLogEntryRowMapper, queries),
                false,
                "get last crawling log of the show with ID '%s'", showId
        );
    }

    @Override
    public void saveCrawlingLogOfTheShow(String showId, List<CrawlLogEntry> log) {
        execute(
                new SaveCrawlingLogOfTheShowTask(showId, log, queries),
                true,
                "save crawling log of the show with ID '%s': %s", showId, log
        );
    }

    @Override
    public Future<String> findShowCrawlerByShowId(String showId) {
        return execute(
                new FindShowCrawlerByShowIdTask(showId, queries),
                false,
                "find crawler of the show with ID '%s'", showId
        );
    }

    @Override
    public void saveShowCrawler(String showId, String showCrawler) {
        execute(
                new SaveShowCrawlerTask(showId, showCrawler, queries),
                true,
                "save crawler of the show with ID '%s': %s", showId, showCrawler
        );
    }

    @Override
    public void deleteShowCrawlerByShowId(String showId) {
        execute(
                new DeleteShowCrawlerByShowId(showId, queries),
                false,
                "delete crawler of the show with ID '%s", showId
        );
    }

    @Override
    public Future<Show> findById(UUID id) {
        return execute(
                new FindShowByIdTask(id, queries, showMetaDataRowMapper, episodeRowMapper),
                false,
                "find show with ID '%s'", id
        );
    }

    @Override
    public Future<List<ShowMetaData>> findAll() {
        return execute(
                new FindAllShowsTask(queries, showMetaDataRowMapper),
                false,
                "find all shows"
        );
    }

    @Override
    public Future<Boolean> containsShowWithName(String name) {
        return execute(
                new DoesShowWithNameExistsTask(name, queries),
                false,
                "check if a show with name '%s' exists", name
        );
    }

    @Override
    public void save(Show show) {
        execute(
                new SaveShowTask(show, queries),
                true,
                "save %s", show
        );
    }

    @Override
    public void deleteById(UUID id) {
        execute(
                new DeleteShowByIdTask(id, queries),
                false,
                "delete show with ID '%s'", id
        );
    }

    @Override
    public Future<List<ShowView>> getShowWatchHistory() {
        return execute(
                new GetShowWatchHistoryTask(showViewRowMapper, queries),
                false,
                "get watch history of all shows"
        );
    }

    @Override
    public Future<List<EpisodeView>> getEpisodeWatchHistoryOfShow(String showId) {
        return execute(
                new GetEpisodeWatchHistoryOfShowTask(showId, episodeViewRowMapper, queries),
                false,
                "get watch history of the show with ID '%s'", showId
        );
    }

    @Override
    public void watchShowEpisode(String showId, String episodeId, WatchProgress watchProgress) {
        execute(
                new WatchShowEpisodeTask(showId, episodeId, watchProgress, queries),
                true,
                "watch episode '%s' of the show with ID '%s' for %s", episodeId, showId, watchProgress
        );
    }

    @Override
    public void clearWatchHistoryOfShow(String showId) {
        execute(
                new ClearWatchHistoryOfShowTask(showId, queries),
                false,
                "clear watch history of the show with ID '%s'", showId
        );
    }

    private <T> Future<T> execute(StorageTask<T> task, boolean transactional, String intentTemplate, Object ...taskArguments) {
        if (transactional) {
            task = new TransactionalTask<>(task, transactionManager);
        }
        task = new ExceptionCatchingTask<>(task, intentTemplate, taskArguments);
        return CompletableFuture.completedFuture(task.execute(template));
    }
}
