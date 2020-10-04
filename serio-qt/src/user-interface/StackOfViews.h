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
    virtual void pushView(const QString& view) const;
    virtual void popCurrentView() const;
    virtual void popAllViews() const;
    virtual void popAllViewsUntil(const QString& view) const;
    virtual void replaceCurrentViewWith(const QString& newView) const;
    virtual void replaceSpecifiedViewWith(const QString& targetView, const QString& newView) const;
public slots:
    void setCurrentView(QString currentView) const;
signals:
    void push(QString view) const;
    void pop(bool currentView) const;
    void unwind(QString targetView) const;
    void replaceCurrentView(QString newView) const;
    void replaceSpecifiedView(QString targetView, QString newView) const;
    void currentViewChanged(QString currentView) const;
private:
    [[nodiscard]] QString makePathToViewAbsolute(const QString& view) const;
};

}


#endif //SERIO_STACKOFVIEWS_H
