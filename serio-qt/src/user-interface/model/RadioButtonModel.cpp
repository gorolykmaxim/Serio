#include "RadioButtonModel.h"
#include <utility>

serio::qt::RadioButtonModel::RadioButtonModel(QString name, bool checked)
    : QObject(),
      name(std::move(name)),
      checked(checked) {}

void serio::qt::RadioButtonModel::setChecked(bool isChecked) {
    checked = isChecked;
    emit checkedChanged();
}

QString serio::qt::RadioButtonModel::getName() const {
    return name;
}

bool serio::qt::RadioButtonModel::isChecked() const {
    return checked;
}
