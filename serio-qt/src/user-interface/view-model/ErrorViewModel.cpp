#include "ErrorViewModel.h"

serio::qt::ErrorViewModel::ErrorViewModel(serio::qt::DialogViewModel& dialog) : dialog(dialog) {}

void serio::qt::ErrorViewModel::initialize(serio::qt::ActionRouter &router) {
    router.registerAction(ActionType::DISPLAY_ERROR, [this] (const auto& args) { displayError(args); });
    router.setErrorAction([this] (const auto& args) { displayError(args); });
}

void serio::qt::ErrorViewModel::displayError(const QVariantList& args) {
    DialogModel model("Whoops...", args[0].toString());
    model.hideBottomButton();
    model.setTopButtonText("understand");
    dialog.display(model, args[1].toBool());
}
