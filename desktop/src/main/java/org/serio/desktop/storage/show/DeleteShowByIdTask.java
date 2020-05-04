package org.serio.desktop.storage.show;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;
import java.util.UUID;

/**
 * Delete the show with the specified ID.
 */
public class DeleteShowByIdTask implements StorageTask<Void> {
    private final String showId;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show to delete
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public DeleteShowByIdTask(UUID showId, Properties queries) {
        this.showId = showId.toString();
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-show-by-id"), showId);
        return null;
    }
}
