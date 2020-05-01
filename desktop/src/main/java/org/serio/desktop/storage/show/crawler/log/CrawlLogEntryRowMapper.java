package org.serio.desktop.storage.show.crawler.log;

import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;

public class CrawlLogEntryRowMapper implements RowMapper<CrawlLogEntry> {
    @Override
    public CrawlLogEntry mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new CrawlLogEntry(
                rs.getString(1),
                rs.getString(2),
                rs.getString(3)
        );
    }
}
