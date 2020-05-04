package org.serio.desktop.storage.show;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

/**
 * Check if the show with the exact specified name exists.
 */
public class DoesShowWithNameExistsTask implements StorageTask<Boolean> {
    private final String showName;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showName the show name to check
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public DoesShowWithNameExistsTask(String showName, Properties queries) {
        this.showName = showName;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Boolean execute(JdbcTemplate template) {
        return template.queryForObject(queries.getProperty("count-shows-with-name"), new Object[]{showName}, Integer.class) > 0;
    }
}
