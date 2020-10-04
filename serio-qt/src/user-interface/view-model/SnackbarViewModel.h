#ifndef SERIO_SNACKBARVIEWMODEL_H
#define SERIO_SNACKBARVIEWMODEL_H

#include "ViewModel.h"
#include <QQmlApplicationEngine>

namespace serio::qt {

class SnackbarViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(bool displayed READ isDisplayed NOTIFY hasChanged)
    Q_PROPERTY(QString text READ getText NOTIFY hasChanged)
public:
    [[nodiscard]] bool isDisplayed() const;
    void initialize(QQmlApplicationEngine& engine);
    virtual void displayText(const QString& textToDisplay);
    [[nodiscard]] QString getText() const;
public slots:
    void hide();
signals:
    void hasChanged() const;
    void hasBeenDisplayed() const;
private:
    bool displayed = false;
    QString text;
};

}

#endif //SERIO_SNACKBARVIEWMODEL_H
