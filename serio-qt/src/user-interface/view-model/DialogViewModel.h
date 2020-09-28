#ifndef SERIO_DIALOGVIEWMODEL_H
#define SERIO_DIALOGVIEWMODEL_H

#include <user-interface/model/DialogModel.h>
#include "ViewModel.h"

namespace serio::qt {

class DialogViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle NOTIFY dialogChanged)
    Q_PROPERTY(QString message READ getMessage NOTIFY dialogChanged)
    Q_PROPERTY(QString rightButtonText READ getRightButtonText NOTIFY dialogChanged)
    Q_PROPERTY(int rightButtonAction READ getRightButtonAction NOTIFY dialogChanged)
    Q_PROPERTY(QString leftButtonText READ getLeftButtonText NOTIFY dialogChanged)
    Q_PROPERTY(int leftButtonAction READ getLeftButtonAction NOTIFY dialogChanged)
    Q_PROPERTY(bool leftButtonHidden READ isLeftButtonHidden NOTIFY dialogChanged)
public:
    explicit DialogViewModel(StackOfViews& stack);
    void initialize(QQmlApplicationEngine& engine);
    virtual void display(const DialogModel& model);
    [[nodiscard]] QString getTitle() const;
    [[nodiscard]] QString getMessage() const;
    [[nodiscard]] QString getRightButtonText() const;
    [[nodiscard]] ActionType getRightButtonAction() const;
    [[nodiscard]] bool isLeftButtonHidden() const;
    [[nodiscard]] QString getLeftButtonText() const;
    [[nodiscard]] ActionType getLeftButtonAction() const;
signals:
    void dialogChanged();
private:
    StackOfViews& stack;
    DialogModel dialogModel;
};

}

#endif //SERIO_DIALOGVIEWMODEL_H
