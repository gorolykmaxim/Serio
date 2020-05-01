package org.serio.desktop.storage;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.support.TransactionTemplate;

public class TransactionalTask<T> implements StorageTask<T> {
    private final StorageTask<T> task;
    private final PlatformTransactionManager platformTransactionManager;

    public TransactionalTask(StorageTask<T> task, PlatformTransactionManager platformTransactionManager) {
        this.task = task;
        this.platformTransactionManager = platformTransactionManager;
    }

    @Override
    public T execute(JdbcTemplate template) {
        return new TransactionTemplate(platformTransactionManager).execute(transactionStatus -> task.execute(template));
    }
}
