#ifndef SERIO_ERRORVIEWMODEL_H
#define SERIO_ERRORVIEWMODEL_H

#include <user-interface/StackOfViews.h>
#include <QQmlApplicationEngine>
#include "ViewModel.h"
#include "DialogViewModel.h"

namespace serio::qt {

class ErrorViewModel : public ViewModel {
public:
    explicit ErrorViewModel(DialogViewModel& dialog);
    void initialize(ActionRouter& router);
    void displayError(const QVariantList& args);
private:
    DialogViewModel& dialog;
};

}

#endif //SERIO_ERRORVIEWMODEL_H
