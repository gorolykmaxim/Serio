#include "SnackbarViewModel.h"
#include <QQmlContext>

void serio::qt::SnackbarViewModel::initialize(QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("snackbarViewModel", this);
}

bool serio::qt::SnackbarViewModel::isDisplayed() const {
    return displayed;
}

void serio::qt::SnackbarViewModel::displayText(const QString& textToDisplay) {
    modifyModel([this, textToDisplay] {
        displayed = true;
        text = textToDisplay;
        emit hasChanged();
        emit hasBeenDisplayed();
    });
}

QString serio::qt::SnackbarViewModel::getText() const {
    return text;
}

void serio::qt::SnackbarViewModel::hide() {
    displayed = false;
    emit hasChanged();
}
