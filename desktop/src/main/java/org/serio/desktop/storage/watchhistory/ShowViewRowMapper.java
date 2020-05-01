package org.serio.desktop.storage.watchhistory;

import org.serio.core.watchhistory.ShowView;
import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.time.Instant;
import java.time.ZoneId;

public class ShowViewRowMapper implements RowMapper<ShowView> {
    @Override
    public ShowView mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new ShowView(
                rs.getString(1),
                Instant.ofEpochMilli(rs.getLong(2)).atZone(ZoneId.systemDefault()).toLocalDateTime()
        );
    }
}
