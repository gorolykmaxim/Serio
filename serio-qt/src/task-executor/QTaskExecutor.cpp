#include "QTaskExecutor.h"

namespace serio::qt {

void QTaskExecutor::runInBackground(const std::function<void()>& task) {
    QtConcurrent::run(&singleTaskPool, task);
}

void QTaskExecutor::runInBackgroundAndWait(const std::function<void()>& task) {
    QtConcurrent::run(&singleTaskPool, task).waitForFinished();
}

}