

#include "model.h"

int main() {
  // std::string s = "Hello";
  // std::cout << s << "| size=" << s.size() << "\n";

  // std::list<std::tuple<double, int, int>> t{{1.1, 1, 1}, {0.1, 0, 0}};
  // std::apply([](auto&&... args) {((std::cout << args << '\n'), ...);},
  // t.back());

  s21::Model x;
  // x.lexem.push_back({1.0, 1, s21::Model::ACOS});
  // x.res_stack.push({1.0, 3});

  // std::cout << x.res_stack.top().first << "\n";
  // std::cout << std::get<0>(x.lexem.front()) << "\n";

  std::string input =
      "asin(0.3) + acos(0.7) - ln(10)*log(10) + tan(0.4)*atan(0.4)";
  // std::cout << input[0];
  x.setInput(input);
  std::cout << "before calc RESULT: " << x.input_string << "\n";
  x.head_calc();
  // std::cout << "VALIDATION RESULT: " << x.validationInput() << "\n";
  std::cout << "after calc  RESULT STRING: " << x.input_string << "\n";
  std::cout << std::get<2>(x.lexem.front()) << "\n";
  std::cout << "lexem count after rpn: " << x.lexem.size() << "\n";
  std::cout << "after calc  RESULT: " << x.result << "\n";
  return 0;
}