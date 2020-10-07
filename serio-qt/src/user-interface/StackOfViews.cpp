#include "StackOfViews.h"
#include <QQmlContext>

void serio::qt::StackOfViews::initialize(ActionRouter& router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("stackOfViews", this);
    router.registerAction(ActionType::BACK, [this] (const auto& args) { popCurrentView(); });
}

void serio::qt::StackOfViews::pushView(const QString &view) const {
    emit push(makePathToViewAbsolute(view));
}

void serio::qt::StackOfViews::popCurrentView() const {
    emit pop(true);
}

void serio::qt::StackOfViews::popAllViews() const {
    emit pop(false);
}

void serio::qt::StackOfViews::popAllViewsUntil(const QString &view) const {
    emit unwind(makePathToViewAbsolute(view));
}

void serio::qt::StackOfViews::replaceCurrentViewWith(const QString& newView) const {
    emit replaceCurrentView(makePathToViewAbsolute(newView));
}

void serio::qt::StackOfViews::replaceSpecifiedViewWith(const QString &targetView, const QString &newView) const {
    emit replaceSpecifiedView(makePathToViewAbsolute(targetView), makePathToViewAbsolute(newView));
}

QString serio::qt::StackOfViews::makePathToViewAbsolute(const QString& view) const {
    return "views/" + view;
}

void serio::qt::StackOfViews::setCurrentView(QString currentView) const {
    emit currentViewChanged(currentView.replace("views/", ""));
}
