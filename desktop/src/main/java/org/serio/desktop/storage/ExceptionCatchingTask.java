package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;

/**
 * A proxy to the actual {@link StorageTask}, that wraps the underlying task into a try-catch and re-throws all the
 * exceptions with a context information attached.
 * @param <T>
 */
public class ExceptionCatchingTask<T> implements StorageTask<T> {
    private final StorageTask<T> task;
    private final String operation;

    /**
     * Construct a task.
     *
     * @param task task to decorate
     * @param intentTemplate human-readable intention of the specified task. Will be used as an exception message. Think
     *                       of it as the first argument in {@link String#format(String, Object...)}
     * @param taskArguments arguments, that should be inserted into the intentTemplate. Think of them as  the second
     *                      argument in {@link String#format(String, Object...)}
     */
    public ExceptionCatchingTask(StorageTask<T> task, String intentTemplate, Object... taskArguments) {
        this.task = task;
        operation = String.format(intentTemplate, taskArguments);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T execute(JdbcTemplate template) {
        try {
            return task.execute(template);
        } catch (Exception e) {
            throw new StorageException(operation, e);
        }
    }
}
