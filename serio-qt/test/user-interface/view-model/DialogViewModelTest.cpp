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

TEST_F(DialogViewModelTest, shouldHaveConfirmAsRightButtonTextByDefault) {
    EXPECT_EQ("confirm", dialog.getRightButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedRightButtonText) {
    QString rightButtonText = "right button text";
    model.setRightButtonText(rightButtonText);
    dialog.display(model);
    EXPECT_EQ(rightButtonText, dialog.getRightButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveBackAsDefaultRightButtonAction) {
    EXPECT_EQ(serio::qt::ActionType::BACK, dialog.getRightButtonAction());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedActionSetAsRightButtonAction) {
    serio::qt::ActionType action = serio::qt::ActionType::OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW;
    model.setRightButtonAction(action);
    dialog.display(model);
    EXPECT_EQ(action, dialog.getRightButtonAction());
}

TEST_F(DialogViewModelTest, shouldDisplayLeftButtonByDefault) {
    EXPECT_FALSE(dialog.isLeftButtonHidden());
}

TEST_F(DialogViewModelTest, shouldHideLeftButton) {
    model.hideLeftButton();
    dialog.display(model);
    EXPECT_TRUE(dialog.isLeftButtonHidden());
}

TEST_F(DialogViewModelTest, shouldHaveCancelAsLeftButtonTextByDefault) {
    EXPECT_EQ("cancel", dialog.getLeftButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedLeftButtonText) {
    QString leftButtonText = "left button text";
    model.setLeftButtonText(leftButtonText);
    dialog.display(model);
    EXPECT_EQ(leftButtonText, dialog.getLeftButtonText());
}

TEST_F(DialogViewModelTest, shouldHaveBackAsDefaultLeftButtonAction) {
    EXPECT_EQ(serio::qt::ActionType::BACK, dialog.getLeftButtonAction());
}

TEST_F(DialogViewModelTest, shouldHaveSpecifiedActionAsLeftButtonAction) {
    serio::qt::ActionType action = serio::qt::ActionType::PREVIEW_CRAWLER;
    model.setLeftButtonAction(action);
    dialog.display(model);
    EXPECT_EQ(action, dialog.getLeftButtonAction());
}

TEST_F(DialogViewModelTest, shouldNotifyWatchersAboutChangesInDialogView) {
    QSignalSpy spy(&dialog, &serio::qt::DialogViewModel::dialogChanged);
    dialog.display(model);
    EXPECT_EQ(1, spy.count());
}
