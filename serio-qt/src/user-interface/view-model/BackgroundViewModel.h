#ifndef SERIO_BACKGROUNDVIEWMODEL_H
#define SERIO_BACKGROUNDVIEWMODEL_H

#include "ViewModel.h"

namespace serio::qt {

class BackgroundViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible NOTIFY visibilityChanged)
    Q_PROPERTY(QString image READ getImage NOTIFY imageChanged)
public:
    explicit BackgroundViewModel(QSet<QString> viewsWithBackground = {});
    void initialize(StackOfViews& stack, QQmlApplicationEngine& engine);
    void setCurrentView(const QString& currentView);
    [[nodiscard]] bool isVisible() const;
    [[nodiscard]] QString getImage() const;
    virtual void setImage(const QString& newImage);
signals:
    void visibilityChanged();
    void imageChanged();
private:
    bool visible;
    QString image;
    QSet<QString> viewsWithBackground;
};

}

#endif //SERIO_BACKGROUNDVIEWMODEL_H
