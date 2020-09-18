#ifndef SERIO_ERRORVIEWMODEL_H
#define SERIO_ERRORVIEWMODEL_H

#include <user-interface/StackOfViews.h>
#include <QQmlApplicationEngine>
#include "ViewModel.h"

namespace serio::qt {

class ErrorViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString errorText READ getErrorText NOTIFY errorTextChanged)
public:
    explicit ErrorViewModel(StackOfViews& stack);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    void displayError(const QVariantList& list);
    [[nodiscard]] QString getErrorText() const;
signals:
    void errorTextChanged();
private:
    QString errorText;
    StackOfViews& stack;
};

}

#endif //SERIO_ERRORVIEWMODEL_H
