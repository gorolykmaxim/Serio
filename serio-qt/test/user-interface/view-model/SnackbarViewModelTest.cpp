#include <gtest/gtest.h>
#include <user-interface/view-model/SnackbarViewModel.h>
#include <QSignalSpy>

class SnackbarViewModelTest : public ::testing::Test {
protected:
    const QString text = "Text to display";
    serio::qt::SnackbarViewModel snackbar;
    QSignalSpy spy = QSignalSpy(&snackbar, &serio::qt::SnackbarViewModel::hasChanged);
};

TEST_F(SnackbarViewModelTest, shouldNotBeDisplayedByDefault) {
    EXPECT_FALSE(snackbar.isDisplayed());
}

TEST_F(SnackbarViewModelTest, shouldDisplaySpecifiedText) {
    snackbar.displayText(text);
    EXPECT_TRUE(snackbar.isDisplayed());
    EXPECT_EQ(text, snackbar.getText());
    EXPECT_EQ(1, spy.count());
}

TEST_F(SnackbarViewModelTest, shouldNotifyWatchersAboutTextChange) {
    QSignalSpy displayedSpy(&snackbar, &serio::qt::SnackbarViewModel::hasBeenDisplayed);
    snackbar.displayText(text);
    EXPECT_EQ(1, displayedSpy.count());
}

TEST_F(SnackbarViewModelTest, shouldBecomeHidden) {
    snackbar.displayText("Something");
    snackbar.hide();
    EXPECT_FALSE(snackbar.isDisplayed());
    EXPECT_EQ(2, spy.count());
}

TEST_F(SnackbarViewModelTest, shouldNotClearItsTextOnHide) {
    snackbar.displayText("Something");
    snackbar.hide();
    EXPECT_FALSE(snackbar.getText().isEmpty());
}
