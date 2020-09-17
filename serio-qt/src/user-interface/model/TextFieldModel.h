#ifndef SERIO_TEXTFIELDMODEL_H
#define SERIO_TEXTFIELDMODEL_H

#include <QObject>

namespace serio::qt {

class TextFieldModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY valueChanged)
public:
    explicit TextFieldModel(QString name, QString value = "");
    void setValue(QString newValue);
    [[nodiscard]] QString getName() const;
    [[nodiscard]] QString getValue() const;
signals:
    void valueChanged();
private:
    QString name;
    QString value;
};

}

#endif //SERIO_TEXTFIELDMODEL_H
