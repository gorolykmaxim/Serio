package org.serio.desktop.storage.show;

import org.serio.core.showstorage.Episode;
import org.serio.core.showstorage.Show;
import org.serio.core.showstorage.ShowMetaData;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Properties;
import java.util.UUID;

/**
 * Find the show (including it's episodes) with the specified ID.
 *
 * <p>If there is no show with the specified ID - an exception will be thrown.</p>
 */
public class FindShowByIdTask implements StorageTask<Show> {
    private final String showId;
    private final Properties queries;
    private final RowMapper<ShowMetaData> showMetaDataRowMapper;
    private final RowMapper<Episode> episodeRowMapper;

    /**
     * Construct a task.
     *
     * @param showId ID of the show to find
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     * @param showMetaDataRowMapper {@link ShowMetaData} {@link RowMapper}
     * @param episodeRowMapper {@link Episode} {@link RowMapper}
     */
    public FindShowByIdTask(UUID showId, Properties queries, RowMapper<ShowMetaData> showMetaDataRowMapper,
                            RowMapper<Episode> episodeRowMapper) {
        this.showId = showId.toString();
        this.queries = queries;
        this.showMetaDataRowMapper = showMetaDataRowMapper;
        this.episodeRowMapper = episodeRowMapper;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Show execute(JdbcTemplate template) {
        ShowMetaData showMetaData = template.queryForObject(queries.getProperty("find-show-by-id"), new Object[]{showId}, showMetaDataRowMapper);
        List<Episode> episodes = template.query(queries.getProperty("find-episodes-by-show-id"), new Object[]{showId}, episodeRowMapper);
        return new Show(showMetaData, episodes);
    }
}
