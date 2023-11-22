#ifndef S21_SMARTCALC
#define S21_SMARTCALC
#include <algorithm>
#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

namespace s21 {

class Model {
 public:
  enum Type_t {
    NUMBER,
    X,
    BRACE_OPEN,
    BRACE_CLOSE,
    ARITHMETIC,
    FUNCTION,
    ASIN,
    ACOS,
    ATAN,
    SIN,
    SQRT,
    LOG,
    LN,
    MOD,
    COS,
    TAN,
    PLUS,
    MINUS,
    DIV,
    MULT,
    POW,
    NOTHING
  };

  // typedef struct lexem_struct {
  //   double value;
  //   int priority;
  //   struct lexem_struct* next;
  //   type_t type;
  // } lexem_t;
  // struct res_stack {
  //   double val[255];
  //   int amount;
  // };

 public:
  std::string input_string;
  int x_exist = 0;
  double x_value;
  double result;
  std::list<std::tuple<double, int, Type_t>> lexem;  // value, priority, type
  std::stack<std::pair<double, int>> res_stack;
  Model() = default;
  ~Model() = default;
  int rr;

  // BOUND WITH CONTROLLER
  void setInput(std::string input);
  double getResult();

  // INNER METHODS
  int head_calc();
  int validationInput();
  void removeSpaces();
  int parsingInput();
  int search_num(size_t* i, double* number);
  int search_substr(std::string search_str, size_t len_input_str, size_t* i,
                    size_t count_char_after);
};

}  // namespace s21

#endif  // S21_SMARTCALC