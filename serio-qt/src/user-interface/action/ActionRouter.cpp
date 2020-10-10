#include "ActionRouter.h"
#include <qqml.h>
#include <QQmlContext>
#include <utility>

serio::qt::ActionRouter::ActionRouter(serio::qt::QTaskExecutor &executor) : QObject(), executor(executor) {}

void serio::qt::ActionRouter::initialize(QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableMetaObject(serio::qt::staticMetaObject, "Serio", 1, 0, "ActionType", nullptr);
    engine.rootContext()->setContextProperty("actionRouter", this);
}

void serio::qt::ActionRouter::setErrorAction(std::function<void(const QVariantList &)> action) {
    errorAction = std::move(action);
}

void serio::qt::ActionRouter::registerAction(ActionType type, std::function<void(const QVariantList&)> action) {
    typeToAction[type] = std::move(action);
}

void serio::qt::ActionRouter::trigger(int actionType, const QVariantList& arguments) {
    executor.runInBackground([=] {
        try {
            typeToAction[static_cast<ActionType>(actionType)](arguments);
        } catch (std::runtime_error& e) {
            if (errorAction) {
                errorAction.value()(QVariantList({QString(e.what()), false}));
            }
        }
    });
}
