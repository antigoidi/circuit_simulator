#include "../../src/parser/adder.h"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(example, test1) {
  auto res = add(1.0f, 4.0f);
  ASSERT_EQ(res, 5.0f);
}

TEST(example, test2) {
  auto res = add(2.3f, 4.0f);
  ASSERT_EQ(res, 6.3f);
}