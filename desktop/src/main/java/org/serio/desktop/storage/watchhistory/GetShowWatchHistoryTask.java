package org.serio.desktop.storage.watchhistory;

import org.serio.core.watchhistory.ShowView;
import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.util.List;
import java.util.Properties;

public class GetShowWatchHistoryTask implements StorageTask<List<ShowView>> {
    private final RowMapper<ShowView> showViewRowMapper;
    private final Properties queries;

    public GetShowWatchHistoryTask(RowMapper<ShowView> showViewRowMapper, Properties queries) {
        this.showViewRowMapper = showViewRowMapper;
        this.queries = queries;
    }

    @Override
    public List<ShowView> execute(JdbcTemplate template) {
        return template.query(queries.getProperty("get-all-show-views"), showViewRowMapper);
    }
}
