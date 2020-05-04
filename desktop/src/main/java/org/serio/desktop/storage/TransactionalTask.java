package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.support.TransactionTemplate;

/**
 * A proxy to the actual {@link StorageTask}, that wraps the underlying task into a database transaction.
 *
 * <p>In case the underlying task will throw a {@link RuntimeException} - the transaction will be rolled back.</p>
 * @param <T>
 */
public class TransactionalTask<T> implements StorageTask<T> {
    private final StorageTask<T> task;
    private final PlatformTransactionManager platformTransactionManager;

    /**
     * Construct a task.
     *
     * @param task task to decorate
     * @param platformTransactionManager transaction manager to obtain a transaction from
     */
    public TransactionalTask(StorageTask<T> task, PlatformTransactionManager platformTransactionManager) {
        this.task = task;
        this.platformTransactionManager = platformTransactionManager;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T execute(JdbcTemplate template) {
        return new TransactionTemplate(platformTransactionManager).execute(transactionStatus -> task.execute(template));
    }
}
