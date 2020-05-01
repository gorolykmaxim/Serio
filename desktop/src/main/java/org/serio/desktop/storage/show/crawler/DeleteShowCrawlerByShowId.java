package org.serio.desktop.storage.show.crawler;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

public class DeleteShowCrawlerByShowId implements StorageTask<Void> {
    private final String showId;
    private final Properties queries;

    public DeleteShowCrawlerByShowId(String showId, Properties queries) {
        this.showId = showId;
        this.queries = queries;
    }

    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-show-crawler-by-show-id"), showId);
        return null;
    }
}
