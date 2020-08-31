#include <gtest/gtest.h>
#include <tv-show-storage/ListPage.h>

class ListPageTest : public ::testing::Test {
protected:
    unsigned int expectedOffset = 100;
    unsigned int expectedSize = 1000;
    std::vector<int> items = {0, 1, 2};
    serio::core::ListPage<int> page;
public:
    ListPageTest() : page(expectedOffset, expectedSize, items) {}
};

TEST_F(ListPageTest, shouldHaveSpecifiedOffset) {
    EXPECT_EQ(expectedOffset, page.getOffset());
}

TEST_F(ListPageTest, shouldHaveSpecifiedTotalSize) {
    EXPECT_EQ(expectedSize, page.getTotalSize());
}

TEST_F(ListPageTest, shouldNotOverflowTotalSize) {
    EXPECT_THROW(serio::core::ListPage<int>(11, 10, {}), std::invalid_argument);
    EXPECT_THROW(serio::core::ListPage<int>(10, 10, {1}), std::invalid_argument);
}

TEST_F(ListPageTest, shouldNotContainItemWithTheSpecifiedGlobalIndex) {
    EXPECT_FALSE(page.containsItemWithGlobalIndex(expectedOffset - 1));
    EXPECT_FALSE(page.containsItemWithGlobalIndex(expectedOffset + items.size()));
}

TEST_F(ListPageTest, shouldContainItemWithTheSpecifeidGlobalIndex) {
    EXPECT_TRUE(page.containsItemWithGlobalIndex(expectedOffset));
}

TEST_F(ListPageTest, shouldFailToGetItemThatIsNotPartOfThePage) {
    EXPECT_THROW(page.getItemByGlobalIndex(expectedOffset - 1), std::out_of_range);
    EXPECT_THROW(page.getItemByGlobalIndex(expectedOffset + items.size()), std::out_of_range);
}

TEST_F(ListPageTest, shouldGetItemByItsGlobalIndex) {
    EXPECT_EQ(items.at(0), page.getItemByGlobalIndex(expectedOffset));
}

TEST_F(ListPageTest, pageWithGreaterOffsetShouldBeGreater) {
    serio::core::ListPage<int> a(expectedOffset, expectedSize, {});
    serio::core::ListPage<int> b(expectedOffset + 1, expectedSize, {});
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
}

TEST_F(ListPageTest, twoPagesWithTheSameOffsetShouldBeEqual) {
    EXPECT_LE(page, page);
    EXPECT_GE(page, page);
}

TEST_F(ListPageTest, shouldReturnIndexOfFirstItem) {
    EXPECT_EQ(expectedOffset, page.getFirstItemIndex());
    EXPECT_EQ(13, serio::core::ListPage<int>(13, 100, {}).getFirstItemIndex());
}

TEST_F(ListPageTest, shouldReturnIndexOfLastItem) {
    EXPECT_EQ(expectedOffset + 2, page.getLastItemIndex());
    EXPECT_EQ(5, serio::core::ListPage<int>(2, 10, {0, 1, 2, 3}).getLastItemIndex());
}
