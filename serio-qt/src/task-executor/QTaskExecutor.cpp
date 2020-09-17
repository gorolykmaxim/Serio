#include "QTaskExecutor.h"

void serio::qt::QTaskExecutor::runInBackground(const std::function<void()>& task) {
    QtConcurrent::run(&singleTaskPool, task);
}

void serio::qt::QTaskExecutor::runInBackgroundAndWait(const std::function<void()>& task) {
    QtConcurrent::run(&singleTaskPool, task).waitForFinished();
}
