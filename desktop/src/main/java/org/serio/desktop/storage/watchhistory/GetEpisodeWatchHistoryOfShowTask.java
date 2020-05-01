package org.serio.desktop.storage.watchhistory;

import org.serio.core.watchhistory.EpisodeView;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Properties;

public class GetEpisodeWatchHistoryOfShowTask implements StorageTask<List<EpisodeView>> {
    private final String showId;
    private final RowMapper<EpisodeView> episodeViewRowMapper;
    private final Properties queries;

    public GetEpisodeWatchHistoryOfShowTask(String showId, RowMapper<EpisodeView> episodeViewRowMapper, Properties queries) {
        this.showId = showId;
        this.episodeViewRowMapper = episodeViewRowMapper;
        this.queries = queries;
    }

    @Override
    public List<EpisodeView> execute(JdbcTemplate template) {
        return template.query(queries.getProperty("get-episode-views-by-show-id"), new Object[]{showId}, episodeViewRowMapper);
    }
}
