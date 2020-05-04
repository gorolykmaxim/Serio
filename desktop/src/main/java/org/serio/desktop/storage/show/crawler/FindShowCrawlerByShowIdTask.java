package org.serio.desktop.storage.show.crawler;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

/**
 * Find crawler of the show with the specified ID.
 *
 * <p>This task will throw an exception if the specified show does not have a crawler.</p>
 */
public class FindShowCrawlerByShowIdTask implements StorageTask<String> {
    private final String showId;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, crawler of which should be found
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public FindShowCrawlerByShowIdTask(String showId, Properties queries) {
        this.showId = showId;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String execute(JdbcTemplate template) {
        return template.queryForObject(queries.getProperty("find-show-crawler-by-show-id"), new Object[] {showId}, String.class);
    }
}
