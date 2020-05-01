package org.serio.desktop.storage.show.crawler;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

public class SaveShowCrawlerTask implements StorageTask<Void> {
    private final String showId;
    private final String showCrawler;
    private final Properties queries;

    public SaveShowCrawlerTask(String showId, String showCrawler, Properties queries) {
        this.showId = showId;
        this.showCrawler = showCrawler;
        this.queries = queries;
    }

    @Override
    public Void execute(JdbcTemplate template) {
        new DeleteShowCrawlerByShowId(showId, queries).execute(template);
        template.update(queries.getProperty("save-show-crawler"), showId, showCrawler);
        return null;
    }
}
