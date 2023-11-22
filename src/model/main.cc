

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

  std::string input = "sin(-5)+(2.3)-   sin(3)/ln(7)";
  // std::cout << input[0];
  x.setInput(input);
  x.head_calc();
  std::cout << "RESULT: " << x.input_string << "\n";
  std::cout << "VALIDATION RESULT: " << x.validationInput() << "\n";
  std::cout << "RESULT: " << x.input_string << "\n";
  std::cout << std::get<2>(x.lexem.front()) << "\n";
  return 0;
}