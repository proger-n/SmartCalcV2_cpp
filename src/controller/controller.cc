#include "controller.h"
namespace s21 {

void Controller::setInput(std::string input) { model.setInput(input); }

std::string Controller::getResult() { return model.headCalc(); }

}  // namespace s21