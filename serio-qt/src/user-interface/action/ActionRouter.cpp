#include "ActionRouter.h"
#include <qqml.h>
#include <QQmlContext>
#include <utility>

serio::qt::ActionRouter::ActionRouter(serio::qt::QTaskExecutor &executor, QQmlApplicationEngine& engine)
    : QObject(), executor(executor) {
    qmlRegisterUncreatableMetaObject(serio::qt::staticMetaObject, "Serio", 1, 0, "ActionType", nullptr);
    engine.rootContext()->setContextProperty("actionRouter", this);
}

void serio::qt::ActionRouter::registerAction(ActionType type, std::function<void(const QVariantList&)> action) {
    typeToAction[type] = std::move(action);
}

void serio::qt::ActionRouter::trigger(int actionType, const QVariantList& arguments) {
    executor.runInBackground([=] { typeToAction[static_cast<ActionType>(actionType)](arguments); });
}
