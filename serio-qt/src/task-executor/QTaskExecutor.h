#ifndef SERIO_QTASKEXECUTOR_H
#define SERIO_QTASKEXECUTOR_H

#include <QtConcurrent>
#include <QFutureWatcher>

namespace serio::qt {

class QTaskExecutor {
public:
    QTaskExecutor() {
        singleTaskPool.setMaxThreadCount(1);
        singleTaskPool.setExpiryTimeout(-1);
    }
    template <typename T, typename Class, typename Param1, typename Arg1>
    void runInBackground(QFutureWatcher<T>& watcher, Class* object, T (Class::*func)(Param1), const Arg1& arg1) {
        return watcher.setFuture(QtConcurrent::run(&singleTaskPool, object, func, arg1));
    }
    template <typename T, typename Class, typename Param1, typename Param2, typename Arg1, typename Arg2>
    void runInBackground(QFutureWatcher<T>& watcher, Class* object, T (Class::*func)(Param1, Param2), const Arg1& arg1, const Arg2& arg2) {
        return watcher.setFuture(QtConcurrent::run(&singleTaskPool, object, func, arg1, arg2));
    }
private:
    QThreadPool singleTaskPool;
};

}

#endif //SERIO_QTASKEXECUTOR_H
