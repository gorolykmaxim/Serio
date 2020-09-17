#ifndef SERIO_QTASKEXECUTOR_H
#define SERIO_QTASKEXECUTOR_H

#include <QtConcurrent>
#include <QFutureWatcher>
#include <utility>

namespace serio::qt {

class QTaskExecutor {
public:
    QTaskExecutor() {
        singleTaskPool.setMaxThreadCount(1);
        singleTaskPool.setExpiryTimeout(-1);
    }
    void runInBackground(const std::function<void()>& task);
    void runInBackgroundAndWait(const std::function<void()>& task);
private:
    QThreadPool singleTaskPool;
};

}

#endif //SERIO_QTASKEXECUTOR_H
