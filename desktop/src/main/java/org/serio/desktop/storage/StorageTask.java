package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;

public interface StorageTask<T> {
    T execute(JdbcTemplate template);
}
