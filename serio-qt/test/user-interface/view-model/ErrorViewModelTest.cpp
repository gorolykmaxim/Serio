#include <gtest/gtest.h>
#include <user-interface/view-model/ErrorViewModel.h>
#include <DialogViewModelMock.h>

TEST(ErrorViewModelTest, shouldDisplayErrorMessage) {
    DialogViewModelMock dialog;
    serio::qt::ErrorViewModel viewModel(dialog);
    QString error = "expected error";
    serio::qt::DialogModel model("Whoops...", error);
    model.hideBottomButton();
    model.setTopButtonText("understand");
    EXPECT_CALL(dialog, display(model));
    viewModel.displayError(QVariantList({error}));
}
