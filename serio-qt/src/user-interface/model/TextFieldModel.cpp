#include "TextFieldModel.h"
#include <utility>

namespace serio::qt {

TextFieldModel::TextFieldModel(QString name, QString value) :
    QObject(), name(std::move(name)), value(std::move(value)) {}

void TextFieldModel::setValue(QString newValue) {
    value = std::move(newValue);
    emit valueChanged();
}

QString TextFieldModel::getName() const {
    return name;
}

QString TextFieldModel::getValue() const {
    return value;
}

}