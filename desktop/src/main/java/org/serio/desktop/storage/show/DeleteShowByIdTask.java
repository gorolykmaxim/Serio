package org.serio.desktop.storage.show;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;
import java.util.UUID;

public class DeleteShowByIdTask implements StorageTask<Void> {
    private final String showId;
    private final Properties queries;

    public DeleteShowByIdTask(UUID showId, Properties queries) {
        this.showId = showId.toString();
        this.queries = queries;
    }

    @Override
    public Void execute(JdbcTemplate template) {
        template.update(queries.getProperty("delete-show-by-id"), showId);
        return null;
    }
}
