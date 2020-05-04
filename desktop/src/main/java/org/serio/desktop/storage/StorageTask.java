package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;

/**
 * Base work unit and a building block of the {@link DesktopStorage}.
 *
 * <p>Every operation in the {@link DesktopStorage}, whether it is a query or a modification, is represented as
 * a {@link StorageTask}.</p>
 * @param <T>
 */
public interface StorageTask<T> {
    /**
     * Execute the corresponding SQL against the specified template.
     *
     * @param template template to execute the corresponding SQL with
     * @return result of this task, if such does exist
     */
    T execute(JdbcTemplate template);
}
