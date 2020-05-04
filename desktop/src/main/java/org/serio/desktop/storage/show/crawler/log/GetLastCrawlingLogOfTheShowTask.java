package org.serio.desktop.storage.show.crawler.log;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Optional;
import java.util.Properties;

/**
 * Get all the {@link CrawlLogEntry}s of the last crawl log of the show with the specified ID.
 *
 * <p>If the last crawling log does not have any entries or if there is no log - an empty {@link Optional}
 * will be returned.</p>
 */
public class GetLastCrawlingLogOfTheShowTask implements StorageTask<Optional<List<CrawlLogEntry>>> {
    private final String showId;
    private final RowMapper<CrawlLogEntry> mapper;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, last crawling log of which should be obtained
     * @param mapper {@link CrawlLogEntry} {@link RowMapper}
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public GetLastCrawlingLogOfTheShowTask(String showId, RowMapper<CrawlLogEntry> mapper, Properties queries) {
        this.showId = showId;
        this.mapper = mapper;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Optional<List<CrawlLogEntry>> execute(JdbcTemplate template) {
        List<CrawlLogEntry> lastLog = template.query(queries.getProperty("get-last-crawling-log-of-the-show"), new Object[]{showId}, mapper);
        return lastLog.isEmpty() ? Optional.empty() : Optional.of(lastLog);
    }
}
