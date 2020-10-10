#include <gtest/gtest.h>
#include <user-interface/view-model/ErrorViewModel.h>
#include <DialogViewModelMock.h>

class ErrorViewModelTest : public ::testing::Test {
protected:
    DialogViewModelMock dialog;
    serio::qt::ErrorViewModel viewModel = serio::qt::ErrorViewModel(dialog);
    QString error = "expected error";
    serio::qt::DialogModel model = serio::qt::DialogModel("Whoops...", error);
    virtual void SetUp() {
        model.hideBottomButton();
        model.setTopButtonText("understand");
    }
};

TEST_F(ErrorViewModelTest, shouldDisplayErrorMessage) {
    EXPECT_CALL(dialog, display(model, false));
    viewModel.displayError(QVariantList({error, false}));
}

TEST_F(ErrorViewModelTest, shouldDisplayErrorMessageInPlaceOfCurrentView) {
    EXPECT_CALL(dialog, display(model, true));
    viewModel.displayError(QVariantList({error, true}));
}
