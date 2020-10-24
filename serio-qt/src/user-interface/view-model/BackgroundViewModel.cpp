#include "BackgroundViewModel.h"
#include <QQmlContext>
#include <utility>

namespace serio::qt {

BackgroundViewModel::BackgroundViewModel(QSet<QString> viewsWithBackground)
    : viewsWithBackground(std::move(viewsWithBackground)), visible(false) {}

void BackgroundViewModel::initialize(StackOfViews &stack, QQmlApplicationEngine &engine) {
    connect(&stack, &StackOfViews::currentViewChanged, this, &BackgroundViewModel::setCurrentView);
    engine.rootContext()->setContextProperty("backgroundViewModel", this);
}

bool BackgroundViewModel::isVisible() const {
    return visible;
}

void BackgroundViewModel::setCurrentView(const QString &currentView) {
    auto previouslyVisible = visible;
    visible = viewsWithBackground.contains(currentView);
    if (previouslyVisible != visible) {
        emit visibilityChanged();
    }
}

void BackgroundViewModel::setImage(const QString& newImage) {
    modifyModel([this, newImage] {
        image = newImage;
        emit imageChanged();
    });
}

QString BackgroundViewModel::getImage() const {
    return image;
}

}