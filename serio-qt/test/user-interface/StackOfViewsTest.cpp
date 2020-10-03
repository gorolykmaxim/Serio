#include <gtest/gtest.h>
#include <user-interface/StackOfViews.h>
#include <QSignalSpy>
#include <user-interface/ViewNames.h>

class StackOfViewsTest : public ::testing::Test {
protected:
    serio::qt::StackOfViews stack;
    QSignalSpy pushSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::push);
    QSignalSpy popSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::pop);
};

TEST_F(StackOfViewsTest, shouldPushSingleViewOnStackWithItsAbsolutePath) {
    stack.pushView("View1.qml");
    EXPECT_EQ(QString("views/View1.qml"), pushSpy.takeFirst()[0].toString());
}

TEST_F(StackOfViewsTest, shouldPopCurrentViewFromStack) {
    stack.popCurrentView();
    EXPECT_TRUE(popSpy.takeFirst()[0].toBool());
}

TEST_F(StackOfViewsTest, shouldPopAllViewsFromStack) {
    stack.popAllViews();
    EXPECT_FALSE(popSpy.takeFirst()[0].toBool());
}

TEST_F(StackOfViewsTest, shouldPopAllViewsUntilTheSpecifiedOne) {
    QSignalSpy unwindSpy(&stack, &serio::qt::StackOfViews::unwind);
    stack.popAllViewsUntil("View2.qml");
    EXPECT_EQ(QString("views/View2.qml"), unwindSpy.takeFirst()[0].toString());
}

TEST_F(StackOfViewsTest, shouldReplaceCurrentViewWithTheSpecifiedOne) {
    QSignalSpy replaceSpy(&stack, &serio::qt::StackOfViews::replaceCurrentView);
    stack.replaceCurrentViewWith("View3.qml");
    EXPECT_EQ(QString("views/View3.qml"), replaceSpy.takeFirst()[0].toString());
}

TEST_F(StackOfViewsTest, shouldReplaceSpecifiedViewWithTheSpecifiedNewOne) {
    QSignalSpy replaceSpy(&stack, &serio::qt::StackOfViews::replaceSpecifiedView);
    stack.replaceSpecifiedViewWith("View1.qml", "View3.qml");
    QVariantList args = replaceSpy.takeFirst();
    EXPECT_EQ(QString("views/View1.qml"), args[0].toString());
    EXPECT_EQ(QString("views/View3.qml"), args[1].toString());
}

TEST_F(StackOfViewsTest, shouldNotifyWatchersAboutNewViewBeingDisplayed) {
    QString specifiedView = serio::qt::tvShowView;
    QSignalSpy currentViewSpy(&stack, &serio::qt::StackOfViews::currentViewChanged);
    stack.setCurrentView("views/" + specifiedView);
    ASSERT_EQ(1, currentViewSpy.count());
    EXPECT_EQ(specifiedView, currentViewSpy.takeFirst()[0].toString());
}
