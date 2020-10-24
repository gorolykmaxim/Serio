#include "RadioButtonModel.h"
#include <utility>

namespace serio::qt {

RadioButtonModel::RadioButtonModel(QString name, bool checked)
    : QObject(),
      name(std::move(name)),
      checked(checked) {}

void RadioButtonModel::setChecked(bool isChecked) {
    checked = isChecked;
    emit checkedChanged();
}

QString RadioButtonModel::getName() const {
    return name;
}

bool RadioButtonModel::isChecked() const {
    return checked;
}

}