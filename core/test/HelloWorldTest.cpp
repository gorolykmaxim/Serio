#include <gtest/gtest.h>
#include "HelloWorld.h"

TEST(TestHelloWorld, shouldReturnHelloWorld) {
    ASSERT_EQ("Hello World", serio::core::getHelloWorld());
}
