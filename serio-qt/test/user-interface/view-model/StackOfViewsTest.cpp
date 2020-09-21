#include <gtest/gtest.h>
#include <user-interface/StackOfViews.h>
#include <QSignalSpy>

class StackOfViewsTest : public ::testing::Test {
protected:
    serio::qt::StackOfViews stack;
    QSignalSpy pushSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::push);
    QSignalSpy popSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::pop);
};

TEST_F(StackOfViewsTest, shouldPushMultipleViewsOnStackWithTheirAbsolutePaths) {
    stack.pushViews(QStringList({"View1.qml", "View2.qml"}));
    EXPECT_EQ(QStringList({"views/View1.qml", "views/View2.qml"}), pushSpy.takeFirst()[0].toStringList());
}

TEST_F(StackOfViewsTest, shouldPushSingleViewOnStackWithItsAbsolutePath) {
    stack.pushView("View1.qml");
    EXPECT_EQ(QStringList({"views/View1.qml"}), pushSpy.takeFirst()[0].toStringList());
}

TEST_F(StackOfViewsTest, shouldPopCurrentViewFromStack) {
    stack.popCurrentView();
    EXPECT_TRUE(popSpy.takeFirst()[0].toBool());
}

TEST_F(StackOfViewsTest, shouldPopAllViewsFromStack) {
    stack.popAllViews();
    EXPECT_FALSE(popSpy.takeFirst()[0].toBool());
}

TEST_F(StackOfViewsTest, shouldReplaceCurrentViewWithTheSpecifiedOne) {
    QSignalSpy replaceSpy(&stack, &serio::qt::StackOfViews::replace);
    stack.replaceCurrentViewWith("View3.qml");
    EXPECT_EQ(QString("views/View3.qml"), replaceSpy.takeFirst()[0].toString());
}
