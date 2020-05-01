package org.serio.desktop.storage.show.crawler;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

public class FindShowCrawlerByShowIdTask implements StorageTask<String> {
    private final String showId;
    private final Properties queries;

    public FindShowCrawlerByShowIdTask(String showId, Properties queries) {
        this.showId = showId;
        this.queries = queries;
    }

    @Override
    public String execute(JdbcTemplate template) {
        return template.queryForObject(queries.getProperty("find-show-crawler-by-show-id"), new Object[] {showId}, String.class);
    }
}
