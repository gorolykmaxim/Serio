#include <user-interface/ViewNames.h>
#include <QQmlContext>
#include "DialogViewModel.h"

serio::qt::DialogViewModel::DialogViewModel(serio::qt::StackOfViews &stack) : stack(stack), dialogModel("", "") {}

void serio::qt::DialogViewModel::initialize(QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("dialogViewModel", this);
}

void serio::qt::DialogViewModel::display(const serio::qt::DialogModel& model) {
    modifyModel([this, model] {
        dialogModel = model;
        emit dialogChanged();
        stack.pushView(dialogView);
    });
}

QString serio::qt::DialogViewModel::getTitle() const {
    return dialogModel.getTitle();
}

QString serio::qt::DialogViewModel::getMessage() const {
    return dialogModel.getMessage();
}

QString serio::qt::DialogViewModel::getRightButtonText() const {
    return dialogModel.getRightButtonText();
}

serio::qt::ActionType serio::qt::DialogViewModel::getRightButtonAction() const {
    return dialogModel.getRightButtonAction();
}

bool serio::qt::DialogViewModel::isLeftButtonHidden() const {
    return dialogModel.isLeftButtonHidden();
}

QString serio::qt::DialogViewModel::getLeftButtonText() const {
    return dialogModel.getLeftButtonText();
}

serio::qt::ActionType serio::qt::DialogViewModel::getLeftButtonAction() const {
    return dialogModel.getLeftButtonAction();
}
