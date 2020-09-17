#include "TextFieldModel.h"
#include <utility>

serio::qt::TextFieldModel::TextFieldModel(QString name, QString value) : QObject(), name(std::move(name)), value(std::move(value)) {}

void serio::qt::TextFieldModel::setValue(QString newValue) {
    value = std::move(newValue);
    emit valueChanged();
}

QString serio::qt::TextFieldModel::getName() const {
    return name;
}

QString serio::qt::TextFieldModel::getValue() const {
    return value;
}
