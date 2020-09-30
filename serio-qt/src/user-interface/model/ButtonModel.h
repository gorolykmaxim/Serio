#ifndef SERIO_BUTTONMODEL_H
#define SERIO_BUTTONMODEL_H

#include <QObject>
#include <user-interface/action/ActionType.h>

namespace serio::qt {

class ButtonModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ getText CONSTANT)
    Q_PROPERTY(int clickAction READ getClickAction CONSTANT)
    Q_PROPERTY(bool highlighted READ isHighlighted NOTIFY highlightedChanged)
public:
    ButtonModel(QString text, ActionType clickAction);
    [[nodiscard]] QString getText() const;
    [[nodiscard]] ActionType getClickAction() const;
    [[nodiscard]] bool isHighlighted() const;
    void setHighlighted(bool isHighlighted);
    bool operator==(const ButtonModel &rhs) const;
    bool operator!=(const ButtonModel &rhs) const;
signals:
    void highlightedChanged();
private:
    QString text;
    ActionType clickAction;
    bool highlighted;
};

}

#endif //SERIO_BUTTONMODEL_H
