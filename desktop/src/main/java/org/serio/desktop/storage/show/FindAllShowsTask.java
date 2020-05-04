package org.serio.desktop.storage.show;

import org.serio.core.showstorage.ShowMetaData;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Properties;

/**
 * Find all shows (not including their episodes), stored in the storage.
 */
public class FindAllShowsTask implements StorageTask<List<ShowMetaData>> {
    private final Properties queries;
    private final RowMapper<ShowMetaData> showMetaDataRowMapper;

    /**
     * Construct a task.
     *
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     * @param showMetaDataRowMapper {@link ShowMetaData} {@link RowMapper}
     */
    public FindAllShowsTask(Properties queries, RowMapper<ShowMetaData> showMetaDataRowMapper) {
        this.queries = queries;
        this.showMetaDataRowMapper = showMetaDataRowMapper;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<ShowMetaData> execute(JdbcTemplate template) {
        return template.query(queries.getProperty("find-all-shows"), showMetaDataRowMapper);
    }
}
