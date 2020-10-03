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
    void setTopButtonText(QString newTopButtonText);
    void setTopButtonAction(ActionType actionType);
    [[nodiscard]] QString getTopButtonText() const;
    [[nodiscard]] ActionType getTopButtonAction() const;
    void hideBottomButton();
    [[nodiscard]] bool isBottomButtonHidden() const;
    void setBottomButtonText(QString newBottomButtonText);
    [[nodiscard]] QString getBottomButtonText() const;
    void setBottomButtonAction(ActionType actionType);
    [[nodiscard]] ActionType getBottomButtonAction() const;
    bool operator==(const DialogModel &rhs) const;
    bool operator!=(const DialogModel &rhs) const;
private:
    QString title;
    QString message;
    QString topButtonText;
    ActionType topButtonAction;
    bool bottomButtonHidden;
    QString bottomButtonText;
    ActionType bottomButtonAction;
};

}

#endif //SERIO_DIALOGMODEL_H
