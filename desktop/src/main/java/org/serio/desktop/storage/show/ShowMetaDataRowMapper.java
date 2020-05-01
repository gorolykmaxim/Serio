package org.serio.desktop.storage.show;

import org.serio.core.showstorage.ShowMetaData;
import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

public class ShowMetaDataRowMapper implements RowMapper<ShowMetaData> {
    @Override
    public ShowMetaData mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new ShowMetaData(UUID.fromString(rs.getString(1)), rs.getString(2), rs.getString(3));
    }
}
