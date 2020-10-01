#include "ButtonModel.h"

serio::qt::ButtonModel::ButtonModel(QString text, serio::qt::ActionType clickAction, QVariantList arguments)
    : QObject(), text(std::move(text)), clickAction(clickAction), highlighted(false), clickArguments(std::move(arguments)) {}

QString serio::qt::ButtonModel::getText() const {
    return text;
}

serio::qt::ActionType serio::qt::ButtonModel::getClickAction() const {
    return clickAction;
}

bool serio::qt::ButtonModel::isHighlighted() const {
    return highlighted;
}

serio::qt::ButtonModel& serio::qt::ButtonModel::setHighlighted(bool isHighlighted) {
    highlighted = isHighlighted;
    emit highlightedChanged();
    return *this;
}

QVariantList serio::qt::ButtonModel::getClickArguments() const {
    return clickArguments;
}

bool serio::qt::ButtonModel::operator==(const serio::qt::ButtonModel &rhs) const {
    return text == rhs.text &&
           clickAction == rhs.clickAction &&
           highlighted == rhs.highlighted &&
           clickArguments == rhs.clickArguments;
}

bool serio::qt::ButtonModel::operator!=(const serio::qt::ButtonModel &rhs) const {
    return !(rhs == *this);
}
