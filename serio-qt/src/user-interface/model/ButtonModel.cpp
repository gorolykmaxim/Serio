#include "ButtonModel.h"

namespace serio::qt {

ButtonModel::ButtonModel(QString text, ActionType clickAction, QVariantList arguments, bool isPrimary)
    : QObject(),
      text(std::move(text)),
      clickAction(clickAction),
      highlighted(false),
      clickArguments(std::move(arguments)),
      primary(isPrimary) {}

QString ButtonModel::getText() const {
    return text;
}

ActionType ButtonModel::getClickAction() const {
    return clickAction;
}

bool ButtonModel::isHighlighted() const {
    return highlighted;
}

ButtonModel& ButtonModel::setHighlighted(bool isHighlighted) {
    highlighted = isHighlighted;
    emit highlightedChanged();
    return *this;
}

QVariantList ButtonModel::getClickArguments() const {
    return clickArguments;
}

bool ButtonModel::isPrimary() const {
    return primary;
}

bool ButtonModel::operator==(const ButtonModel &rhs) const {
    return text == rhs.text &&
           clickAction == rhs.clickAction &&
           highlighted == rhs.highlighted &&
           clickArguments == rhs.clickArguments &&
           primary == rhs.primary;
}

bool ButtonModel::operator!=(const ButtonModel &rhs) const {
    return !(rhs == *this);
}

}