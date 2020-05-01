package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;

public class InitializeStorageTask implements StorageTask<Void> {
    private final String initializationScript;

    public InitializeStorageTask(String initializationScript) {
        this.initializationScript = initializationScript;
    }

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
