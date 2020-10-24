#include "StackOfViews.h"
#include <QQmlContext>

namespace serio::qt {

void StackOfViews::initialize(ActionRouter& router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("stackOfViews", this);
    router.registerAction(ActionType::BACK, [this] (const auto& args) { popCurrentView(); });
}

void StackOfViews::pushView(const QString &view) const {
    emit push(makePathToViewAbsolute(view));
}

void StackOfViews::popCurrentView() const {
    emit pop(true);
}

void StackOfViews::popAllViews() const {
    emit pop(false);
}

void StackOfViews::popAllViewsUntil(const QString &view) const {
    emit unwind(makePathToViewAbsolute(view));
}

void StackOfViews::replaceCurrentViewWith(const QString& newView) const {
    emit replaceCurrentView(makePathToViewAbsolute(newView));
}

void StackOfViews::replaceSpecifiedViewWith(const QString &targetView, const QString &newView) const {
    emit replaceSpecifiedView(makePathToViewAbsolute(targetView), makePathToViewAbsolute(newView));
}

QString StackOfViews::makePathToViewAbsolute(const QString& view) const {
    return "views/" + view;
}

void StackOfViews::setCurrentView(QString currentView) const {
    emit currentViewChanged(currentView.replace("views/", ""));
}

}