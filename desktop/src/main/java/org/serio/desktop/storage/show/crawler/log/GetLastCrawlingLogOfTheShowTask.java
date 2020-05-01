package org.serio.desktop.storage.show.crawler.log;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Optional;
import java.util.Properties;

public class GetLastCrawlingLogOfTheShowTask implements StorageTask<Optional<List<CrawlLogEntry>>> {
    private final String showId;
    private final RowMapper<CrawlLogEntry> mapper;
    private final Properties queries;

    public GetLastCrawlingLogOfTheShowTask(String showId, RowMapper<CrawlLogEntry> mapper, Properties queries) {
        this.showId = showId;
        this.mapper = mapper;
        this.queries = queries;
    }

    @Override
    public Optional<List<CrawlLogEntry>> execute(JdbcTemplate template) {
        List<CrawlLogEntry> lastLog = template.query(queries.getProperty("get-last-crawling-log-of-the-show"), new Object[]{showId}, mapper);
        return lastLog.isEmpty() ? Optional.empty() : Optional.of(lastLog);
    }
}
