#include "BackgroundViewModel.h"
#include <QQmlContext>
#include <utility>

serio::qt::BackgroundViewModel::BackgroundViewModel(QSet<QString> viewsWithBackground)
    : viewsWithBackground(std::move(viewsWithBackground)), visible(false) {}

void serio::qt::BackgroundViewModel::initialize(serio::qt::StackOfViews &stack, QQmlApplicationEngine &engine) {
    connect(&stack, &serio::qt::StackOfViews::currentViewChanged, this, &serio::qt::BackgroundViewModel::setCurrentView);
    engine.rootContext()->setContextProperty("backgroundViewModel", this);
}

bool serio::qt::BackgroundViewModel::isVisible() const {
    return visible;
}

void serio::qt::BackgroundViewModel::setCurrentView(const QString &currentView) {
    bool previouslyVisible = visible;
    visible = viewsWithBackground.contains(currentView);
    if (previouslyVisible != visible) {
        emit visibilityChanged();
    }
}

void serio::qt::BackgroundViewModel::setImage(const QString& newImage) {
    modifyModel([this, newImage] {
        image = newImage;
        emit imageChanged();
    });
}

QString serio::qt::BackgroundViewModel::getImage() const {
    return image;
}
