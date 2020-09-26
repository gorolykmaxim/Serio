#include "ErrorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

serio::qt::ErrorViewModel::ErrorViewModel(serio::qt::StackOfViews &stack) : stack(stack) {}

void serio::qt::ErrorViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine& engine) {
    engine.rootContext()->setContextProperty("errorViewModel", this);
    router.setErrorAction([this] (const QVariantList& args) { displayError(args); });
}

void serio::qt::ErrorViewModel::displayError(const QVariantList& args) {
    modifyModel([this, args] {
        errorText = args[0].toString();
        emit errorTextChanged();
        stack.pushView(errorView);
    });
}

QString serio::qt::ErrorViewModel::getErrorText() const {
    return errorText;
}
