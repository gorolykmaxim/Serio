#include <user-interface/ViewNames.h>
#include <QQmlContext>
#include "DialogViewModel.h"

namespace serio::qt {

DialogViewModel::DialogViewModel(StackOfViews &stack) : stack(stack), dialogModel("", "") {}

void DialogViewModel::initialize(QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("dialogViewModel", this);
}

void DialogViewModel::display(const DialogModel& model, bool replaceCurrentView) {
    modifyModel([this, model, replaceCurrentView] {
        dialogModel = model;
        emit dialogChanged();
        if (replaceCurrentView) {
            stack.replaceCurrentViewWith(dialogView);
        } else {
            stack.pushView(dialogView);
        }
    });
}

QString DialogViewModel::getTitle() const {
    return dialogModel.getTitle();
}

QString DialogViewModel::getMessage() const {
    return dialogModel.getMessage();
}

QString DialogViewModel::getTopButtonText() const {
    return dialogModel.getTopButtonText();
}

ActionType DialogViewModel::getTopButtonAction() const {
    return dialogModel.getTopButtonAction();
}

bool DialogViewModel::isBottomButtonHidden() const {
    return dialogModel.isBottomButtonHidden();
}

QString DialogViewModel::getBottomButtonText() const {
    return dialogModel.getBottomButtonText();
}

ActionType DialogViewModel::getBottomButtonAction() const {
    return dialogModel.getBottomButtonAction();
}

}