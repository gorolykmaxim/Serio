#ifndef SERIO_VIEWMODEL_H
#define SERIO_VIEWMODEL_H

#include <task-executor/QTaskExecutor.h>
#include <user-interface/StackOfViews.h>

namespace serio::qt {

class ViewModel : public QObject {
    Q_OBJECT
protected:
    void modifyModel(const std::function<void()>& modify);
};

}

#endif //SERIO_VIEWMODEL_H
