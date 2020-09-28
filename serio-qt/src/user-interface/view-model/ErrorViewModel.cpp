#include "ErrorViewModel.h"

serio::qt::ErrorViewModel::ErrorViewModel(serio::qt::DialogViewModel& dialog) : dialog(dialog) {}

void serio::qt::ErrorViewModel::initialize(serio::qt::ActionRouter &router) {
    router.setErrorAction([this] (const QVariantList& args) { displayError(args); });
}

void serio::qt::ErrorViewModel::displayError(const QVariantList& args) {
    DialogModel model("Whoops...", args[0].toString());
    model.hideLeftButton();
    model.setRightButtonText("understand");
    dialog.display(model);
}
