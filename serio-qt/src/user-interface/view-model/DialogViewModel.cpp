#include <user-interface/ViewNames.h>
#include <QQmlContext>
#include "DialogViewModel.h"

serio::qt::DialogViewModel::DialogViewModel(serio::qt::StackOfViews &stack) : stack(stack), dialogModel("", "") {}

void serio::qt::DialogViewModel::initialize(QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("dialogViewModel", this);
}

void serio::qt::DialogViewModel::display(const serio::qt::DialogModel& model, bool replaceCurrentView) {
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

QString serio::qt::DialogViewModel::getTitle() const {
    return dialogModel.getTitle();
}

QString serio::qt::DialogViewModel::getMessage() const {
    return dialogModel.getMessage();
}

QString serio::qt::DialogViewModel::getTopButtonText() const {
    return dialogModel.getTopButtonText();
}

serio::qt::ActionType serio::qt::DialogViewModel::getTopButtonAction() const {
    return dialogModel.getTopButtonAction();
}

bool serio::qt::DialogViewModel::isBottomButtonHidden() const {
    return dialogModel.isBottomButtonHidden();
}

QString serio::qt::DialogViewModel::getBottomButtonText() const {
    return dialogModel.getBottomButtonText();
}

serio::qt::ActionType serio::qt::DialogViewModel::getBottomButtonAction() const {
    return dialogModel.getBottomButtonAction();
}
