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
    virtual void pushView(const QString& view);
    virtual void popCurrentView();
    virtual void popAllViews();
    virtual void popAllViewsUntil(const QString& view);
    virtual void replaceCurrentViewWith(const QString& newView);
    virtual void replaceSpecifiedViewWith(const QString& targetView, const QString& newView);
public slots:
    void setCurrentView(QString currentView);
signals:
    void push(QString view);
    void pop(bool currentView);
    void unwind(QString targetView);
    void replaceCurrentView(QString newView);
    void replaceSpecifiedView(QString targetView, QString newView);
    void currentViewChanged(QString currentView);
private:
    [[nodiscard]] QString makePathToViewAbsolute(const QString& view) const;
};

}


#endif //SERIO_STACKOFVIEWS_H
