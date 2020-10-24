#include "ErrorViewModel.h"

namespace serio::qt {

ErrorViewModel::ErrorViewModel(DialogViewModel& dialog) : dialog(dialog) {}

void ErrorViewModel::initialize(ActionRouter &router) {
    router.registerAction(ActionType::DISPLAY_ERROR, [this] (const auto& args) { displayError(args); });
    router.setErrorAction([this] (const auto& args) { displayError(args); });
}

void ErrorViewModel::displayError(const QVariantList& args) {
    DialogModel model("Whoops...", args[0].toString());
    model.hideBottomButton();
    model.setTopButtonText("understand");
    dialog.display(model, args[1].toBool());
}

}