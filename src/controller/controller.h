#ifndef CPP3_SMARTCALC_V2_0_CONTROLLER_CONTROLLER_H_
#define CPP3_SMARTCALC_V2_0_CONTROLLER_CONTROLLER_H_

#include "../model/model.h"
namespace s21 {
class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  void SetInput(std::string input);
  void SetX(double x_val);
  std::string GetResult();

 private:
  Model model_;
};
}  // namespace s21
#endif  // CPP3_SMARTCALC_V2_0_CONTROLLER_CONTROLLER_H_