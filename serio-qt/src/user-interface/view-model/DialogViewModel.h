#ifndef SERIO_DIALOGVIEWMODEL_H
#define SERIO_DIALOGVIEWMODEL_H

#include <user-interface/model/DialogModel.h>
#include "ViewModel.h"

namespace serio::qt {

class DialogViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle NOTIFY dialogChanged)
    Q_PROPERTY(QString message READ getMessage NOTIFY dialogChanged)
    Q_PROPERTY(QString topButtonText READ getTopButtonText NOTIFY dialogChanged)
    Q_PROPERTY(int topButtonAction READ getTopButtonAction NOTIFY dialogChanged)
    Q_PROPERTY(QString bottomButtonText READ getBottomButtonText NOTIFY dialogChanged)
    Q_PROPERTY(int bottomButtonAction READ getBottomButtonAction NOTIFY dialogChanged)
    Q_PROPERTY(bool bottomButtonHidden READ isBottomButtonHidden NOTIFY dialogChanged)
public:
    explicit DialogViewModel(StackOfViews& stack);
    void initialize(QQmlApplicationEngine& engine);
    virtual void display(const DialogModel& model, bool replaceCurrentView = false);
    [[nodiscard]] QString getTitle() const;
    [[nodiscard]] QString getMessage() const;
    [[nodiscard]] QString getTopButtonText() const;
    [[nodiscard]] ActionType getTopButtonAction() const;
    [[nodiscard]] bool isBottomButtonHidden() const;
    [[nodiscard]] QString getBottomButtonText() const;
    [[nodiscard]] ActionType getBottomButtonAction() const;
signals:
    void dialogChanged() const;
private:
    StackOfViews& stack;
    DialogModel dialogModel;
};

}

#endif //SERIO_DIALOGVIEWMODEL_H
