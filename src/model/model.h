#ifndef S21_SMARTCALC
#define S21_SMARTCALC

namespace s21 {

class Model {
 private:
  typedef enum {
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
  } type_t;

  typedef struct lexem_struct {
    double value;
    int priority;
    struct lexem_struct* next;
    type_t type;
  } lexem_t;
  struct res_stack {
    double val[255];
    int amount;
  };

 public:
  Model() = default;
  ~Model() = default;
};

}  // namespace s21

#endif  // S21_SMARTCALC