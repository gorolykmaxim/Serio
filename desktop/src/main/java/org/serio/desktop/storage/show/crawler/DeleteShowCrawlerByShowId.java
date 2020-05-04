package org.serio.desktop.storage.show.crawler;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

/**
 * Delete crawler of the show with the specified ID.
 */
public class DeleteShowCrawlerByShowId implements StorageTask<Void> {
    private final String showId;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, crawler of which should be deleted
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public DeleteShowCrawlerByShowId(String showId, Properties queries) {
        this.showId = showId;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-show-crawler-by-show-id"), showId);
        return null;
    }
}
