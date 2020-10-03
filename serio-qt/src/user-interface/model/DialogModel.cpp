#include "DialogModel.h"

serio::qt::DialogModel::DialogModel(QString title, QString message)
    : title(std::move(title)),
      message(std::move(message)),
      topButtonText("confirm"),
      topButtonAction(serio::qt::ActionType::BACK),
      bottomButtonHidden(false),
      bottomButtonText("cancel"),
      bottomButtonAction(serio::qt::ActionType::BACK) {}

QString serio::qt::DialogModel::getTitle() const {
    return title;
}

QString serio::qt::DialogModel::getMessage() const {
    return message;
}

void serio::qt::DialogModel::setTopButtonText(QString newTopButtonText) {
    topButtonText = std::move(newTopButtonText);
}

void serio::qt::DialogModel::setTopButtonAction(serio::qt::ActionType actionType) {
    topButtonAction = actionType;
}

QString serio::qt::DialogModel::getTopButtonText() const {
    return topButtonText;
}

serio::qt::ActionType serio::qt::DialogModel::getTopButtonAction() const {
    return topButtonAction;
}

void serio::qt::DialogModel::hideBottomButton() {
    bottomButtonHidden = true;
}

bool serio::qt::DialogModel::isBottomButtonHidden() const {
    return bottomButtonHidden;
}

void serio::qt::DialogModel::setBottomButtonText(QString newBottomButtonText) {
    bottomButtonText = std::move(newBottomButtonText);
}

QString serio::qt::DialogModel::getBottomButtonText() const {
    return bottomButtonText;
}

void serio::qt::DialogModel::setBottomButtonAction(serio::qt::ActionType actionType) {
    bottomButtonAction = actionType;
}

serio::qt::ActionType serio::qt::DialogModel::getBottomButtonAction() const {
    return bottomButtonAction;
}

bool serio::qt::DialogModel::operator==(const serio::qt::DialogModel &rhs) const {
    return title == rhs.title &&
           message == rhs.message &&
           topButtonText == rhs.topButtonText &&
           topButtonAction == rhs.topButtonAction &&
           bottomButtonHidden == rhs.bottomButtonHidden &&
           bottomButtonText == rhs.bottomButtonText &&
           bottomButtonAction == rhs.bottomButtonAction;
}

bool serio::qt::DialogModel::operator!=(const serio::qt::DialogModel &rhs) const {
    return !(rhs == *this);
}
