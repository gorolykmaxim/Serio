package org.serio.desktop.storage.show;

import org.serio.desktop.storage.StorageTask;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.Properties;

public class DoesShowWithNameExistsTask implements StorageTask<Boolean> {
    private final String showName;
    private final Properties queries;

    public DoesShowWithNameExistsTask(String showName, Properties queries) {
        this.showName = showName;
        this.queries = queries;
    }

    @Override
    public Boolean execute(JdbcTemplate template) {
        return template.queryForObject(queries.getProperty("count-shows-with-name"), new Object[]{showName}, Integer.class) > 0;
    }
}
