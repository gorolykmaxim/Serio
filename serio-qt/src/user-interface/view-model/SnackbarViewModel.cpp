#include "SnackbarViewModel.h"
#include <QQmlContext>

namespace serio::qt {

void SnackbarViewModel::initialize(QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("snackbarViewModel", this);
}

bool SnackbarViewModel::isDisplayed() const {
    return displayed;
}

void SnackbarViewModel::displayText(const QString& textToDisplay) {
    modifyModel([this, textToDisplay] {
        displayed = true;
        text = textToDisplay;
        emit hasChanged();
        emit hasBeenDisplayed();
    });
}

QString SnackbarViewModel::getText() const {
    return text;
}

void SnackbarViewModel::hide() {
    displayed = false;
    emit hasChanged();
}

}