#include "DialogModel.h"

serio::qt::DialogModel::DialogModel(QString title, QString message)
    : title(std::move(title)),
      message(std::move(message)),
      rightButtonText("confirm"),
      rightButtonAction(serio::qt::ActionType::BACK),
      leftButtonHidden(false),
      leftButtonText("cancel"),
      leftButtonAction(serio::qt::ActionType::BACK) {}

QString serio::qt::DialogModel::getTitle() const {
    return title;
}

QString serio::qt::DialogModel::getMessage() const {
    return message;
}

void serio::qt::DialogModel::setRightButtonText(QString newRightButtonText) {
    rightButtonText = std::move(newRightButtonText);
}

void serio::qt::DialogModel::setRightButtonAction(serio::qt::ActionType actionType) {
    rightButtonAction = actionType;
}

QString serio::qt::DialogModel::getRightButtonText() const {
    return rightButtonText;
}

serio::qt::ActionType serio::qt::DialogModel::getRightButtonAction() const {
    return rightButtonAction;
}

void serio::qt::DialogModel::hideLeftButton() {
    leftButtonHidden = true;
}

bool serio::qt::DialogModel::isLeftButtonHidden() const {
    return leftButtonHidden;
}

void serio::qt::DialogModel::setLeftButtonText(QString newLeftButtonText) {
    leftButtonText = std::move(newLeftButtonText);
}

QString serio::qt::DialogModel::getLeftButtonText() const {
    return leftButtonText;
}

void serio::qt::DialogModel::setLeftButtonAction(serio::qt::ActionType actionType) {
    leftButtonAction = actionType;
}

serio::qt::ActionType serio::qt::DialogModel::getLeftButtonAction() const {
    return leftButtonAction;
}

bool serio::qt::DialogModel::operator==(const serio::qt::DialogModel &rhs) const {
    return title == rhs.title &&
           message == rhs.message &&
           rightButtonText == rhs.rightButtonText &&
           rightButtonAction == rhs.rightButtonAction &&
           leftButtonHidden == rhs.leftButtonHidden &&
           leftButtonText == rhs.leftButtonText &&
           leftButtonAction == rhs.leftButtonAction;
}

bool serio::qt::DialogModel::operator!=(const serio::qt::DialogModel &rhs) const {
    return !(rhs == *this);
}
