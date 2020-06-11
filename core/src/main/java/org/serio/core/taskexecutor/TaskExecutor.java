package org.serio.core.taskexecutor;

import java.io.Closeable;
import java.io.IOException;
import java.util.concurrent.*;

/**
 * Module that is able to execute time-consuming tasks in parallel.
 */
public class TaskExecutor implements Closeable {
    private ExecutorService singleThreadExecutor, multiThreadExecutor;

    /**
     * Construct an executor module.
     */
    public TaskExecutor() {
        singleThreadExecutor = Executors.newSingleThreadExecutor();
        multiThreadExecutor = Executors.newCachedThreadPool();
    }

    /**
     * Execute specified task asynchronously while providing a guarantee that only one instance of a task,
     * executed by this method, will be executed at a time:
     * <pre>
     *     executeOneAtATime(a);
     *     executeOneAtATime(b); // b will start executing only after a is finished.
     * </pre>
     *
     * @param task task to execute
     */
    public void executeOneAtATime(Runnable task) {
        singleThreadExecutor.execute(task);
    }

    /**
     * Execute specified task asynchronously. Tasks executed by this method are executed in parallel.
     *
     * @param task task to execute
     * @param <T>  type of a value, return by the task upon completion
     * @return future of a task return value
     */
    public <T> Future<T> execute(Callable<T> task) {
        return multiThreadExecutor.submit(task);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void close() throws IOException {
        singleThreadExecutor.shutdownNow();
        multiThreadExecutor.shutdownNow();
    }
}
