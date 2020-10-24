#include "ActionRouter.h"
#include <qqml.h>
#include <QQmlContext>
#include <utility>

namespace serio::qt {

ActionRouter::ActionRouter(QTaskExecutor &executor) : QObject(), executor(executor) {}

void ActionRouter::initialize(QQmlApplicationEngine &engine) {
    qmlRegisterUncreatableMetaObject(serio::qt::staticMetaObject, "Serio", 1, 0,
                                     "ActionType", nullptr);
    engine.rootContext()->setContextProperty("actionRouter", this);
}

void ActionRouter::setErrorAction(std::function<void(const QVariantList &)> action) {
    errorAction = std::move(action);
}

void ActionRouter::registerAction(ActionType type, std::function<void(const QVariantList&)> action) {
    typeToAction[type] = std::move(action);
}

void ActionRouter::trigger(int actionType, const QVariantList& arguments) {
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

}