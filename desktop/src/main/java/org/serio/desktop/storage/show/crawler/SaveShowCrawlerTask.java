package org.serio.desktop.storage.show.crawler;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

/**
 * Save crawler of the show with the specified ID.
 *
 * <p>If there is an already existing crawler of the specified show, stored in the storage - it will be overridden.</p>
 */
public class SaveShowCrawlerTask implements StorageTask<Void> {
    private final String showId;
    private final String showCrawler;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, crawler of which should be saved
     * @param showCrawler show crawler to save
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public SaveShowCrawlerTask(String showId, String showCrawler, Properties queries) {
        this.showId = showId;
        this.showCrawler = showCrawler;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        new DeleteShowCrawlerByShowId(showId, queries).execute(template);
        template.update(queries.getProperty("save-show-crawler"), showId, showCrawler);
        return null;
    }
}
