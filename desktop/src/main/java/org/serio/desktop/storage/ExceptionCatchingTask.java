package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;

public class ExceptionCatchingTask<T> implements StorageTask<T> {
    private final StorageTask<T> task;
    private final String operation;

    public ExceptionCatchingTask(StorageTask<T> task, String intentTemplate, Object... taskArguments) {
        this.task = task;
        operation = String.format(intentTemplate, taskArguments);
    }

    @Override
    public T execute(JdbcTemplate template) {
        try {
            return task.execute(template);
        } catch (Exception e) {
            throw new StorageException(operation, e);
        }
    }
}
