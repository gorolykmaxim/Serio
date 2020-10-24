#include "DialogModel.h"

namespace serio::qt {

DialogModel::DialogModel(QString title, QString message)
    : title(std::move(title)),
      message(std::move(message)),
      topButtonText("confirm"),
      topButtonAction(ActionType::BACK),
      bottomButtonHidden(false),
      bottomButtonText("cancel"),
      bottomButtonAction(ActionType::BACK) {}

QString DialogModel::getTitle() const {
    return title;
}

QString DialogModel::getMessage() const {
    return message;
}

void DialogModel::setTopButtonText(QString newTopButtonText) {
    topButtonText = std::move(newTopButtonText);
}

void DialogModel::setTopButtonAction(ActionType actionType) {
    topButtonAction = actionType;
}

QString DialogModel::getTopButtonText() const {
    return topButtonText;
}

ActionType DialogModel::getTopButtonAction() const {
    return topButtonAction;
}

void DialogModel::hideBottomButton() {
    bottomButtonHidden = true;
}

bool DialogModel::isBottomButtonHidden() const {
    return bottomButtonHidden;
}

void DialogModel::setBottomButtonText(QString newBottomButtonText) {
    bottomButtonText = std::move(newBottomButtonText);
}

QString DialogModel::getBottomButtonText() const {
    return bottomButtonText;
}

void DialogModel::setBottomButtonAction(ActionType actionType) {
    bottomButtonAction = actionType;
}

ActionType DialogModel::getBottomButtonAction() const {
    return bottomButtonAction;
}

bool DialogModel::operator==(const DialogModel &rhs) const {
    return title == rhs.title &&
           message == rhs.message &&
           topButtonText == rhs.topButtonText &&
           topButtonAction == rhs.topButtonAction &&
           bottomButtonHidden == rhs.bottomButtonHidden &&
           bottomButtonText == rhs.bottomButtonText &&
           bottomButtonAction == rhs.bottomButtonAction;
}

bool DialogModel::operator!=(const DialogModel &rhs) const {
    return !(rhs == *this);
}

}