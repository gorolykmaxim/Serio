#ifndef SERIO_DIALOGMODEL_H
#define SERIO_DIALOGMODEL_H

#include <QString>
#include <user-interface/action/ActionType.h>

namespace serio::qt {

class DialogModel {
public:
    DialogModel(QString title, QString message);
    [[nodiscard]] QString getTitle() const;
    [[nodiscard]] QString getMessage() const;
    void setRightButtonText(QString newRightButtonText);
    void setRightButtonAction(ActionType actionType);
    [[nodiscard]] QString getRightButtonText() const;
    [[nodiscard]] ActionType getRightButtonAction() const;
    void hideLeftButton();
    [[nodiscard]] bool isLeftButtonHidden() const;
    void setLeftButtonText(QString newLeftButtonText);
    [[nodiscard]] QString getLeftButtonText() const;
    void setLeftButtonAction(ActionType actionType);
    [[nodiscard]] ActionType getLeftButtonAction() const;
    bool operator==(const DialogModel &rhs) const;
    bool operator!=(const DialogModel &rhs) const;
private:
    QString title;
    QString message;
    QString rightButtonText;
    ActionType rightButtonAction;
    bool leftButtonHidden;
    QString leftButtonText;
    ActionType leftButtonAction;
};

}

#endif //SERIO_DIALOGMODEL_H
