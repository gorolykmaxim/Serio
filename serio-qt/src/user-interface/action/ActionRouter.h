#ifndef SERIO_ACTIONROUTER_H
#define SERIO_ACTIONROUTER_H

#include <QObject>
#include <optional>
#include <task-executor/QTaskExecutor.h>
#include <QQmlApplicationEngine>
#include "ActionType.h"

namespace serio::qt {

class ActionRouter : public QObject {
    Q_OBJECT
public:
    explicit ActionRouter(QTaskExecutor &executor);
    void initialize(QQmlApplicationEngine& engine);
    void setErrorAction(std::function<void(const QVariantList&)> action);
    void registerAction(ActionType type, std::function<void(const QVariantList&)> action);
public slots:
    void trigger(int actionType, const QVariantList& arguments);
private:
    QTaskExecutor& executor;
    std::optional<std::function<void(const QVariantList&)>> errorAction;
    std::map<ActionType, std::function<void(const QVariantList&)>> typeToAction;
};

}

#endif //SERIO_ACTIONROUTER_H
