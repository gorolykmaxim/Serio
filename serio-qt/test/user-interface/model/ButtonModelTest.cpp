#include <gtest/gtest.h>
#include <user-interface/model/ButtonModel.h>
#include <QSignalSpy>

class ButtonModelTest : public ::testing::Test {
protected:
    const QString text = "Click Me!";
    serio::qt::ButtonModel button = serio::qt::ButtonModel(text, serio::qt::ActionType::OPEN_ADD_TV_SHOW_VIEW);
};

TEST_F(ButtonModelTest, shouldCreateButtonWithSpecifiedText) {
    EXPECT_EQ(text, button.getText());
}

TEST_F(ButtonModelTest, shouldCreateButtonWithSpecifiedClickAction) {
    EXPECT_EQ(serio::qt::ActionType::OPEN_ADD_TV_SHOW_VIEW, button.getClickAction());
}

TEST_F(ButtonModelTest, shouldCreateNotHighlightedButtonByDefault) {
    EXPECT_FALSE(button.isHighlighted());
}

TEST_F(ButtonModelTest, shouldNotifyWatchersAboutButtonHighlightmentStatusChange) {
    QSignalSpy spy(&button, &serio::qt::ButtonModel::highlightedChanged);
    button.setHighlighted(true);
    EXPECT_EQ(1, spy.count());
}

TEST_F(ButtonModelTest, shouldIfButtonIsHighlightedOrNot) {
    button.setHighlighted(true);
    EXPECT_TRUE(button.isHighlighted());
    button.setHighlighted(false);
    EXPECT_FALSE(button.isHighlighted());
}

TEST_F(ButtonModelTest, twoButtonsWithSameNameActionAndHighlightmentShouldBeEqual) {
    serio::qt::ButtonModel anotherButton(button.getText(), button.getClickAction());
    EXPECT_EQ(button, anotherButton);
}

TEST_F(ButtonModelTest, twoButtonsShouldNotBeEqualIfASingleAttributeDiffers) {
    EXPECT_NE(button, serio::qt::ButtonModel("", button.getClickAction()));
    EXPECT_NE(button, serio::qt::ButtonModel(button.getText(), button.getClickAction(), {1, 2}));
    EXPECT_NE(button, serio::qt::ButtonModel(button.getText(), serio::qt::ActionType::LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME));
    EXPECT_NE(button, serio::qt::ButtonModel(button.getText(), button.getClickAction(), {}, false));
    button.setHighlighted(true);
    EXPECT_NE(button, serio::qt::ButtonModel(button.getText(), button.getClickAction()));
}

TEST_F(ButtonModelTest, shouldHaveEmptyListOfClickArgumentsByDefault) {
    EXPECT_TRUE(button.getClickArguments().isEmpty());
}

TEST_F(ButtonModelTest, shouldReturnSpecifiedListOfClickArguments) {
    QVariantList arguments = {1, "text", true};
    serio::qt::ButtonModel button(text, serio::qt::ActionType::LOAD_WATCHED_TV_SHOWS_LIST_PAGE, arguments);
    EXPECT_EQ(arguments, button.getClickArguments());
}

TEST_F(ButtonModelTest, shouldCreatePrimaryButtonByDefault) {
    EXPECT_TRUE(button.isPrimary());
}

TEST_F(ButtonModelTest, shouldCreateSecondaryButton) {
    serio::qt::ButtonModel button(text, serio::qt::ActionType::SAVE_CRAWLER_STEP, {}, false);
    EXPECT_FALSE(button.isPrimary());
}
