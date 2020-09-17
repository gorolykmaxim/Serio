#include "StackOfViews.h"
#include <QQmlContext>

void serio::qt::StackOfViews::initialize(ActionRouter& router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("stackOfViews", this);
    router.registerAction(ActionType::BACK, [this] (const QVariantList& args) { popCurrentView(); });
}

void serio::qt::StackOfViews::pushView(const QString &view) {
    pushViews(QStringList(view));
}

void serio::qt::StackOfViews::pushViews(const QStringList& views) {
    emit push(makePathsToViewsAbsolute(views));
}

void serio::qt::StackOfViews::popCurrentView() {
    emit pop(true);
}

void serio::qt::StackOfViews::popAllViews() {
    emit pop(false);
}

QStringList serio::qt::StackOfViews::makePathsToViewsAbsolute(QStringList views) const {
    for (QString& view: views) {
        view = "views/" + view;
    }
    return views;
}
