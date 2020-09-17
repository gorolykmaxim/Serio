#ifndef SERIO_RADIOBUTTONMODEL_H
#define SERIO_RADIOBUTTONMODEL_H

#include <QObject>

namespace serio::qt {

class RadioButtonModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(bool checked READ isChecked NOTIFY checkedChanged)
public:
    RadioButtonModel(QString name, bool checked);
    void setChecked(bool isChecked);
    [[nodiscard]] QString getName() const;
    [[nodiscard]] bool isChecked() const;
signals:
    void checkedChanged();
private:
    QString name;
    bool checked;
};

}

#endif //SERIO_RADIOBUTTONMODEL_H
