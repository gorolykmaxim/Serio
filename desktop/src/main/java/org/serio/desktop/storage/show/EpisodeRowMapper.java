package org.serio.desktop.storage.show;

import org.serio.core.showstorage.Episode;
import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;

public class EpisodeRowMapper implements RowMapper<Episode> {
    @Override
    public Episode mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new Episode(rs.getInt(1), rs.getString(2), rs.getString(3));
    }
}
