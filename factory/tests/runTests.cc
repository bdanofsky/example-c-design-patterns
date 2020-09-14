#include "gtest/gtest.h"
#include "tests/factoryTest.h"
#include "tests/singletonTest.h"
#include "tests/optionsTest.h"
#include "tests/trialsTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
