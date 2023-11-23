#ifndef EXAMPLECONTROLLER_H
#define EXAMPLECONTROLLER_H

#include "../model/model.h"
namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  void setInput(std::string input);
  void setX(std::string input);
  std::string getResult();

 private:
  Model model;
};
}  // namespace s21
#endif