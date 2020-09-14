#include <gtest/gtest.h>
#include <tv-show-storage/BigListProxy.h>
#include <tv-show-storage/ListPage.h>

class BigListProxyTest : public ::testing::Test {
protected:
    serio::core::BigListProxy<int> proxy;
public:
    BigListProxyTest() : proxy(100, 2) {}
};

TEST_F(BigListProxyTest, shouldBeEmptyByDefault) {
    EXPECT_EQ(0, proxy.getTotalSize());
}

TEST_F(BigListProxyTest, shouldHaveTotalSizeOfLastSpecifiedPage) {
    serio::core::ListPage<int> page(0, 5, {});
    proxy.addPage(page);
    EXPECT_EQ(page.getTotalSize(), proxy.getTotalSize());
}

TEST_F(BigListProxyTest, shouldNotContainItemIfNonOfItsPagesContainsItem) {
    EXPECT_FALSE(proxy.containsItemWithGlobalIndex(0));
}

TEST_F(BigListProxyTest, shouldContainItemIfOneOfItsPagesContainsItem) {
    proxy.addPage(serio::core::ListPage<int>(50, 100, {0, 1, 2}));
    proxy.addPage(serio::core::ListPage<int>(95, 100, {0, 1, 2}));
    EXPECT_TRUE(proxy.containsItemWithGlobalIndex(50));
    EXPECT_TRUE(proxy.containsItemWithGlobalIndex(96));
}

TEST_F(BigListProxyTest, shouldFailToGetItemThatDoesNotBelongToAnyOfPages) {
    EXPECT_THROW(proxy.getItemByGlobalIndex(0), std::out_of_range);
}

TEST_F(BigListProxyTest, shouldGetItemThatDoesNotBelongToAnyOfPages) {
    proxy.addPage(serio::core::ListPage<int>(60, 100, {3, 4, 5}));
    proxy.addPage(serio::core::ListPage<int>(50, 100, {0, 1, 2}));
    EXPECT_EQ(1, proxy.getItemByGlobalIndex(51));
}

TEST_F(BigListProxyTest, shouldGetOffsetOfPageContainingItem) {
    EXPECT_EQ(300, proxy.getOffsetOfPageWithIndex(352));
    EXPECT_EQ(400, proxy.getOffsetOfPageWithIndex(441));
}

TEST_F(BigListProxyTest, shouldDisposeTheOldestPageWhenPageAdditionExceedsPageCountLimit) {
    proxy.addPage(serio::core::ListPage<int>(0, 10, {0}));
    proxy.addPage(serio::core::ListPage<int>(1, 10, {1}));
    proxy.addPage(serio::core::ListPage<int>(2, 10, {2}));
    EXPECT_FALSE(proxy.containsItemWithGlobalIndex(0));
}

TEST_F(BigListProxyTest, shouldDisposePageThatHasBeenReloadedWithNewItems) {
    proxy.addPage(serio::core::ListPage<int>(0, 10, {0, 1, 2, 3, 4}));
    proxy.addPage(serio::core::ListPage<int>(0, 10, {5, 6, 7, 8, 9}));
    EXPECT_EQ(5, proxy.getItemByGlobalIndex(0));
}
