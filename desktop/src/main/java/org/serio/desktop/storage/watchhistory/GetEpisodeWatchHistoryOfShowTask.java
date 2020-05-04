package org.serio.desktop.storage.watchhistory;

import org.serio.core.watchhistory.EpisodeView;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Properties;

/**
 * Get list of views of episodes of the show with the specified ID. Each watched episode will have a corresponding view.
 */
public class GetEpisodeWatchHistoryOfShowTask implements StorageTask<List<EpisodeView>> {
    private final String showId;
    private final RowMapper<EpisodeView> episodeViewRowMapper;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, episode views of which should be obtained
     * @param episodeViewRowMapper {@link EpisodeView} {@link RowMapper}
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public GetEpisodeWatchHistoryOfShowTask(String showId, RowMapper<EpisodeView> episodeViewRowMapper, Properties queries) {
        this.showId = showId;
        this.episodeViewRowMapper = episodeViewRowMapper;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public List<EpisodeView> execute(JdbcTemplate template) {
        return template.query(queries.getProperty("get-episode-views-by-show-id"), new Object[]{showId}, episodeViewRowMapper);
    }
}
