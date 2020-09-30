#include "ButtonModel.h"

serio::qt::ButtonModel::ButtonModel(QString text, serio::qt::ActionType clickAction)
    : QObject(), text(std::move(text)), clickAction(clickAction), highlighted(false) {}

QString serio::qt::ButtonModel::getText() const {
    return text;
}

serio::qt::ActionType serio::qt::ButtonModel::getClickAction() const {
    return clickAction;
}

bool serio::qt::ButtonModel::isHighlighted() const {
    return highlighted;
}

void serio::qt::ButtonModel::setHighlighted(bool isHighlighted) {
    highlighted = isHighlighted;
    emit highlightedChanged();
}

bool serio::qt::ButtonModel::operator==(const serio::qt::ButtonModel &rhs) const {
    return text == rhs.text &&
           clickAction == rhs.clickAction &&
           highlighted == rhs.highlighted;
}

bool serio::qt::ButtonModel::operator!=(const serio::qt::ButtonModel &rhs) const {
    return !(rhs == *this);
}
