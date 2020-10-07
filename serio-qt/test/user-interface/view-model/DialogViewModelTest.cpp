#include <gtest/gtest.h>
#include <user-interface/model/DialogModel.h>
#include <user-interface/view-model/DialogViewModel.h>
#include <StackOfViewsMock.h>
#include <user-interface/ViewNames.h>
#include <QSignalSpy>

class DialogViewModelTest : public ::testing::Test {
protected:
    const QString title = "Dialog title";
    const QString message = "Dialog message";
    serio::qt::DialogModel model = serio::qt::DialogModel(title, message);
    ::testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::DialogViewModel dialog = serio::qt::DialogViewModel(stack);
};

TEST_F(DialogViewModelTest, shouldHaveEmptyTitleByDefault) {
    EXPECT_TRUE(dialog.getTitle().isEmpty());
}

TEST_F(DialogViewModelTest, shouldHaveEmptyMessageByDefault) {
    EXPECT_TRUE(dialog.getMessage().isEmpty());
}

TEST_F(DialogViewModelTest, shouldPushDialogViewToStack) {
    EXPECT_CALL(stack, pushView(serio::qt::dialogView));
    dialog.display(model);
}

TEST_F(DialogViewModelTest, shouldSetTitleToTheSpecifiedOne) {
    dialog.display(model);
    EXPECT_EQ(title, dialog.getTitle());
}

TEST_F(DialogViewModelTest, shouldSetMessageToTheSpecifiedOne) {
    dialog.display(model);
    EXPECT_EQ(message, dialog.getMessage());
}

TEST_F(DialogViewModelTest, shouldHaveConfirmAsTopButtonTextByDefault) {
    EXPECT_EQ("confirm", dialog.getTopButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedTopButtonText) {
    auto rightButtonText = "right button text";
    model.setTopButtonText(rightButtonText);
    dialog.display(model);
    EXPECT_EQ(rightButtonText, dialog.getTopButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveBackAsDefaultTopButtonAction) {
    EXPECT_EQ(serio::qt::ActionType::BACK, dialog.getTopButtonAction());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedActionSetAsTopButtonAction) {
    auto action = serio::qt::ActionType::OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW;
    model.setTopButtonAction(action);
    dialog.display(model);
    EXPECT_EQ(action, dialog.getTopButtonAction());
}

TEST_F(DialogViewModelTest, shouldDisplayBottomButtonByDefault) {
    EXPECT_FALSE(dialog.isBottomButtonHidden());
}

TEST_F(DialogViewModelTest, shouldHideBottomButton) {
    model.hideBottomButton();
    dialog.display(model);
    EXPECT_TRUE(dialog.isBottomButtonHidden());
}

TEST_F(DialogViewModelTest, shouldHaveCancelAsBottomButtonTextByDefault) {
    EXPECT_EQ("cancel", dialog.getBottomButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedBottomButtonText) {
    auto leftButtonText = "left button text";
    model.setBottomButtonText(leftButtonText);
    dialog.display(model);
    EXPECT_EQ(leftButtonText, dialog.getBottomButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveBackAsDefaultBottomButtonAction) {
    EXPECT_EQ(serio::qt::ActionType::BACK, dialog.getBottomButtonAction());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedActionAsBottomButtonAction) {
    auto action = serio::qt::ActionType::PREVIEW_CRAWLER;
    model.setBottomButtonAction(action);
    dialog.display(model);
    EXPECT_EQ(action, dialog.getBottomButtonAction());
}

TEST_F(DialogViewModelTest, shouldNotifyWatchersAboutChangesInDialogView) {
    QSignalSpy spy(&dialog, &serio::qt::DialogViewModel::dialogChanged);
    dialog.display(model);
    EXPECT_EQ(1, spy.count());
}
