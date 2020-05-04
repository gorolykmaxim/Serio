package org.serio.desktop.storage.watchhistory;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

/**
 * Clear watch history of the show with the specified ID.
 */
public class ClearWatchHistoryOfShowTask implements StorageTask<Void> {
    private final String showId;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, watch history of which should be erased
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public ClearWatchHistoryOfShowTask(String showId, Properties queries) {
        this.showId = showId;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-episode-views-by-show-id"), showId);
        return null;
    }
}
