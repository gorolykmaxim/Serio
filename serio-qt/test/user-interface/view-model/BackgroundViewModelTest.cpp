#include <gtest/gtest.h>
#include <user-interface/view-model/BackgroundViewModel.h>
#include <user-interface/ViewNames.h>
#include <QSignalSpy>

class BackgroundViewModelTest : public ::testing::Test {
protected:
    const QString image = "https://tv-show/thumbnail.jpg";
    serio::qt::BackgroundViewModel background = serio::qt::BackgroundViewModel({
        serio::qt::tvShowView, serio::qt::crawlingInProgressView
    });
};

TEST_F(BackgroundViewModelTest, shouldNotDisplayBackgroundByDefault) {
    EXPECT_FALSE(background.isVisible());
}

TEST_F(BackgroundViewModelTest, shouldDisplayBackgroundOnceOneOfSpecifiedViewsGetsDisplayed) {
    background.setCurrentView(serio::qt::tvShowView);
    EXPECT_TRUE(background.isVisible());
    background.setCurrentView(serio::qt::crawlingInProgressView);
    EXPECT_TRUE(background.isVisible());
}

TEST_F(BackgroundViewModelTest, shouldNotifyWatchersAboutVisibilityChange) {
    QSignalSpy visibilitySpy(&background, &serio::qt::BackgroundViewModel::visibilityChanged);
    background.setCurrentView(serio::qt::tvShowView);
    EXPECT_EQ(1, visibilitySpy.count());
}

TEST_F(BackgroundViewModelTest, shouldBecomeInvisibleAfterWhenViewWithoutBackgroundDisplayed) {
    background.setCurrentView(serio::qt::crawlerStepEditorView);
    EXPECT_FALSE(background.isVisible());
}

TEST_F(BackgroundViewModelTest, shouldNotNotifyWatchersIfVisibilityHasNotChangedBetweenViewTransitions) {
    QSignalSpy visibilitySpy(&background, &serio::qt::BackgroundViewModel::visibilityChanged);
    background.setCurrentView(serio::qt::tvShowView);
    background.setCurrentView(serio::qt::crawlingInProgressView);
    EXPECT_EQ(1, visibilitySpy.count());
}

TEST_F(BackgroundViewModelTest, shouldDisplayNoImageByDefault) {
    EXPECT_TRUE(background.getImage().isEmpty());
}

TEST_F(BackgroundViewModelTest, shouldDisplaySpecifiedImage) {
    background.setImage(image);
    EXPECT_EQ(image, background.getImage());
}

TEST_F(BackgroundViewModelTest, shouldNotifyWatchersAboutNewImageBeingDisplayed) {
    QSignalSpy imageSpy(&background, &serio::qt::BackgroundViewModel::imageChanged);
    background.setImage(image);
    EXPECT_EQ(1, imageSpy.count());
}
