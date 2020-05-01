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

public class FindShowByIdTask implements StorageTask<Show> {
    private final String showId;
    private final Properties queries;
    private final RowMapper<ShowMetaData> showMetaDataRowMapper;
    private final RowMapper<Episode> episodeRowMapper;

    public FindShowByIdTask(UUID showId, Properties queries, RowMapper<ShowMetaData> showMetaDataRowMapper,
                            RowMapper<Episode> episodeRowMapper) {
        this.showId = showId.toString();
        this.queries = queries;
        this.showMetaDataRowMapper = showMetaDataRowMapper;
        this.episodeRowMapper = episodeRowMapper;
    }

    @Override
    public Show execute(JdbcTemplate template) {
        ShowMetaData showMetaData = template.queryForObject(queries.getProperty("find-show-by-id"), new Object[]{showId}, showMetaDataRowMapper);
        List<Episode> episodes = template.query(queries.getProperty("find-episodes-by-show-id"), new Object[]{showId}, episodeRowMapper);
        return new Show(showMetaData, episodes);
    }
}
