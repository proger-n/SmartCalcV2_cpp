#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>

#include "model.h"

int main() {
  std::string s = "Hello";
  std::cout << s << "| size=" << s.size() << "\n";

  std::list<std::tuple<double, int, int>> t{{1.1, 1, 1}, {0.1, 0, 0}};
  // for (auto& elem : l)
  //   std::cout << elem << ", ";

  // std::tuple t{42, 'a', 4.2}; // Another C++17 feature: class template argument deduction
  std::apply([](auto&&... args) {((std::cout << args << '\n'), ...);}, t.back());
  return 0;
}