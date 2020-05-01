package org.serio.desktop.storage.watchhistory;

import org.serio.core.watchhistory.EpisodeView;
import org.serio.core.watchhistory.WatchProgress;
import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.time.Instant;
import java.time.ZoneId;

public class EpisodeViewRowMapper implements RowMapper<EpisodeView> {
    @Override
    public EpisodeView mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new EpisodeView(
                rs.getString(1),
                WatchProgress.fromPercentage(rs.getDouble(2)),
                Instant.ofEpochMilli(rs.getLong(3)).atZone(ZoneId.systemDefault()).toLocalDateTime()
        );
    }
}
