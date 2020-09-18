#include <gtest/gtest.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/ErrorViewModel.h>
#include <QSignalSpy>

class ErrorViewModelTest : public ::testing::Test {
protected:
    serio::qt::StackOfViews stack;
    serio::qt::ErrorViewModel viewModel = serio::qt::ErrorViewModel(stack);
};

TEST_F(ErrorViewModelTest, shouldPushErrorViewToStack) {
    QSignalSpy spy(&stack, &serio::qt::StackOfViews::push);
    viewModel.displayError(QVariantList({""}));
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(QStringList("views/ErrorView.qml"), args[0].toStringList());
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
