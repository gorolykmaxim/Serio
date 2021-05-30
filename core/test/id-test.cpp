#include <gtest/gtest.h>
#include <id.h>

class id_test : public ::testing::Test {
protected:
    id_seed seed;
};

TEST_F(id_test, should_generate_id_starting_from_one) {
    EXPECT_EQ(1, create_id(seed));
}

TEST_F(id_test, should_generate_new_ids_that_are_greater_then_previous_ones) {
    EXPECT_LT(create_id(seed), create_id(seed));
}
