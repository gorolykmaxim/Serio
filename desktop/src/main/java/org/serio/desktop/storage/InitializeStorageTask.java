package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;

/**
 * Initializes the storage, by executing all the ';'-separated statements statements of the specified
 * initialization SQL script.
 *
 * <p>This task is fail-fast, which means if one task will fail - all the following tasks won't get executed.</p>
 */
public class InitializeStorageTask implements StorageTask<Void> {
    private final String initializationScript;

    /**
     * Construct a task.
     *
     * @param initializationScript SQL initialization script to execute
     */
    public InitializeStorageTask(String initializationScript) {
        this.initializationScript = initializationScript;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Void execute(JdbcTemplate template) {
        for (String script: initializationScript.split(";")) {
            if (!script.isEmpty()) {
                template.execute(script);
            }
        }
        return null;
    }
}
