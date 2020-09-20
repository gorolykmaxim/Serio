#include <gtest/gtest.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/AddTvShowViewModel.h>
#include <QSignalSpy>

TEST(AddTvShowViewModelTest, shouldPushAddTvShowViewOntoStack) {
    serio::qt::StackOfViews stack;
    QSignalSpy spy(&stack, &serio::qt::StackOfViews::push);
    serio::qt::AddTvShowViewModel viewModel(stack);
    viewModel.openView();
    ASSERT_EQ(1, spy.count());
    QVariantList args = spy.takeFirst();
    EXPECT_EQ(QStringList("views/AddTvShowView.qml"), args[0].toStringList());
}