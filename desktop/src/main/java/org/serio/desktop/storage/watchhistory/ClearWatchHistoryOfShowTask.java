package org.serio.desktop.storage.watchhistory;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

public class ClearWatchHistoryOfShowTask implements StorageTask<Void> {
    private final String showId;
    private final Properties queries;

    public ClearWatchHistoryOfShowTask(String showId, Properties queries) {
        this.showId = showId;
        this.queries = queries;
    }

    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-episode-views-by-show-id"), showId);
        return null;
    }
}
