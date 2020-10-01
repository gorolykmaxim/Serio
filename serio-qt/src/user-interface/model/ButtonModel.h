#ifndef SERIO_BUTTONMODEL_H
#define SERIO_BUTTONMODEL_H

#include <QObject>
#include <QVariantList>
#include <user-interface/action/ActionType.h>

namespace serio::qt {

class ButtonModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ getText CONSTANT)
    Q_PROPERTY(int clickAction READ getClickAction CONSTANT)
    Q_PROPERTY(bool highlighted READ isHighlighted NOTIFY highlightedChanged)
    Q_PROPERTY(QVariantList clickArguments READ getClickArguments CONSTANT)
public:
    ButtonModel(QString text, ActionType clickAction, QVariantList arguments = {});
    [[nodiscard]] QString getText() const;
    [[nodiscard]] ActionType getClickAction() const;
    [[nodiscard]] bool isHighlighted() const;
    ButtonModel& setHighlighted(bool isHighlighted);
    [[nodiscard]] QVariantList getClickArguments() const;
    bool operator==(const ButtonModel &rhs) const;
    bool operator!=(const ButtonModel &rhs) const;
signals:
    void highlightedChanged();
private:
    QString text;
    ActionType clickAction;
    bool highlighted;
    QVariantList clickArguments;
};

}

#endif //SERIO_BUTTONMODEL_H
