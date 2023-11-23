#include "tests.h"

#include <gtest/gtest.h>

#include "../controller/controller.h"

TEST(TestSmartCalc, TestBinaryCalculations) {
  std::string result;
  s21::Controller controller;
  for (int i = 0; i < 10; ++i) {
    std::string input(kBinaryCalculations[i]);
    controller.SetInput(input);
    controller.SetX(1);
    result = controller.GetResult();
    std::string r(kBinaryCalculationsResults[i]);
    EXPECT_EQ(result, r);
  }
}

TEST(TestSmartCalc, TestFunctionsCalculations) {
  std::string result;
  s21::Controller controller;
  for (int i = 0; i < 10; ++i) {
    std::string input(kFunctionCalculations[i]);
    controller.SetInput(input);
    result = controller.GetResult();
    std::string r(kFunctionCalculationsResults[i]);
    EXPECT_EQ(result, r);
  }
}

TEST(TestSmartCalc, TestIncorrectExpressions) {
  std::string result;
  s21::Controller controller;
  for (int i = 0; i < 10; ++i) {
    std::string input(kIncorrectExpressions[i]);
    controller.SetInput(input);
    result = controller.GetResult();
    std::string r("ERROR");
    EXPECT_EQ(result, r);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}