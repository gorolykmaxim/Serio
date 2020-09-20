#ifndef SERIO_STACKOFVIEWS_H
#define SERIO_STACKOFVIEWS_H

#include <QObject>
#include <QVariant>
#include <QQmlApplicationEngine>
#include <user-interface/action/ActionRouter.h>

namespace serio::qt {

class StackOfViews : public QObject {
    Q_OBJECT
public:
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    void pushView(const QString& view);
    void pushViews(const QStringList& views);
    void popCurrentView();
    void popAllViews();
    void replaceCurrentViewWith(const QString& newView);
signals:
    void push(QStringList views);
    void pop(bool currentView);
    void replace(QString newView);
private:
    [[nodiscard]] QStringList makePathsToViewsAbsolute(QStringList views) const;
    [[nodiscard]] QString makePathToViewAbsolute(const QString& view) const;
};

}


#endif //SERIO_STACKOFVIEWS_H
