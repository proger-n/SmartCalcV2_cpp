#ifndef CPP3_SMARTCALC_V2_0_MODEL_MODEL_H_
#define CPP3_SMARTCALC_V2_0_MODEL_MODEL_H_
#include <list>
#include <string>
#include <tuple>
#include <vector>

namespace s21 {
enum TypeT {
  kNumber,
  kX,
  kBraceOpen,
  kBraceClose,
  kArithmetic,
  kFunction,
  kAsin,
  kAcos,
  kAtan,
  kSin,
  kSqrt,
  kLog,
  kLn,
  kMod,
  kCos,
  kTan,
  kPlus,
  kMinus,
  kDiv,
  kMult,
  kPow
};

class Model {
 public:
  Model() = default;
  ~Model() = default;

  void SetInput(std::string input);
  void SetX(double x_val);
  std::string GetResult();

 private:
  int ValidationInput();
  void RemoveSpaces();
  int ParsingInput();
  int SearchNum(size_t* i, double* number);
  int SearchSubstr(std::string search_str, size_t len_input_str, size_t* i,
                   size_t count_char_after);
  int SearchOperand(size_t i, TypeT* en, int* priority);
  int RPNAndCalculate();
  int Calculate();
  int IsFuncUnary(TypeT type);
  double UnaryFuncUsing(TypeT type, double val);
  int IsFuncBinary(TypeT type);
  double BinaryFuncUsing(TypeT type, double val_1, double val_2);

  std::string input_string_;
  int x_exist_ = 0;
  double x_value_;
  double result_ = 0;
  std::list<std::tuple<double, int, TypeT>> lexem_;  // value, priority, type
  std::vector<double> res_stack_;
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_MODEL_MODEL_H_