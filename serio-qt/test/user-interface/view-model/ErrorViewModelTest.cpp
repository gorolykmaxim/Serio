#include <gtest/gtest.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/ErrorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>

class ErrorViewModelTest : public ::testing::Test {
protected:
    ::testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::ErrorViewModel viewModel = serio::qt::ErrorViewModel(stack);
};

TEST_F(ErrorViewModelTest, shouldPushErrorViewToStack) {
    EXPECT_CALL(stack, pushView(QString("ErrorView.qml")));
    viewModel.displayError(QVariantList({""}));
}

TEST_F(ErrorViewModelTest, shouldDisplayErrorMessage) {
    QString errorText = "expected error message to be displayed";
    viewModel.displayError(QVariantList({errorText}));
    EXPECT_EQ(errorText, viewModel.getErrorText());
}

TEST_F(ErrorViewModelTest, shouldNotifyAboutChangedErrorText) {
    QSignalSpy spy(&viewModel, &serio::qt::ErrorViewModel::errorTextChanged);
    viewModel.displayError(QVariantList({""}));
    EXPECT_EQ(1, spy.count());
}
