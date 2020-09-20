#include "AddTvShowViewModel.h"

serio::qt::AddTvShowViewModel::AddTvShowViewModel(serio::qt::StackOfViews &stack) : stack(stack) {}

void serio::qt::AddTvShowViewModel::initialize(serio::qt::ActionRouter &router) {
    router.registerAction(ActionType::OPEN_ADD_TV_SHOW_VIEW, [this] (const QVariantList& args) { openView(); });
}

void serio::qt::AddTvShowViewModel::openView() {
    stack.pushView("AddTvShowView.qml");
}
