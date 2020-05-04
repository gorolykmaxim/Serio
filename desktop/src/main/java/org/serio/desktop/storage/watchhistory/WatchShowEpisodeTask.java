package org.serio.desktop.storage.watchhistory;

import org.serio.core.watchhistory.WatchProgress;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.Properties;

/**
 * Create a new or update an existing {@link org.serio.core.watchhistory.EpisodeView} of the specified episode
 * of the specified show.
 */
public class WatchShowEpisodeTask implements StorageTask<Void> {
    private final String showId;
    private final String episodeId;
    private final WatchProgress progress;
    private final Properties queries;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, episode of which has just been watched
     * @param episodeId ID of the episode, that has just been watched
     * @param progress current {@link WatchProgress} of the specified episode
     * @param queries {@link Properties}, that contains the SQL queries, used by this task
     */
    public WatchShowEpisodeTask(String showId, String episodeId, WatchProgress progress, Properties queries) {
        this.showId = showId;
        this.episodeId = episodeId;
        this.progress = progress;
        this.queries = queries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        long now = LocalDateTime.now().atZone(ZoneId.systemDefault()).toInstant().toEpochMilli();
        double progressPercentage = progress.getPercentage();
        int rowsUpdated = template.update(queries.getProperty("update-episode-view"), now, progressPercentage, showId, episodeId);
        if (rowsUpdated == 0) {
            template.update(queries.getProperty("create-episode-view"), showId, episodeId, now, progressPercentage);
        }
        return null;
    }
}
