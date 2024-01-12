#include "controller.h"
namespace s21 {

void Controller::SetInput(std::string input) { model_.SetInput(input); }

void Controller::SetX(double x_val) { model_.SetX(x_val); }

std::string Controller::GetResult() { return model_.GetResult(); }

}  // namespace s21