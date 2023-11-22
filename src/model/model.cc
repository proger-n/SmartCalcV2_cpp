#include "model.h"
namespace s21 {
int Model::head_calc() {
  int ret = 0;
  std::cout << "INPUT: " << input_string << "\n";
  if (!validationInput() && !parsingInput()) {
    std::cout << "lexem count before rpn: " << lexem.size() << "\n";
    if (rpn_and_calculate()) ret = 1;
  }
  return ret;
}

void Model::setInput(std::string input) { input_string = input; }
double Model::getResult() { return result; }

int Model::validationInput() {
  removeSpaces();
  int ret = 0;
  int brace_opened = 0;
  std::string available_values = "0123456789.,x*/+-^()mdcosinltaqrtg";
  int input_len = input_string.size();
  for (int i = 0; i < input_len && brace_opened >= 0; i++) {
    if (available_values.find(input_string[i]) == std::string::npos) {
      ret = 1;
    } else if (input_string[i] == '(') {
      brace_opened += 1;
    } else if (input_string[i] == ')') {
      brace_opened -= 1;
      if (i > 0 &&
          std::string("+-/*(^").find(input_string[i - 1]) != std::string::npos)
        brace_opened = -1;
    } else if (std::string("/*^").find(input_string[i]) != std::string::npos) {
      if (i + 1 == input_len) ret = 1;
      if (i > 0 &&
          std::string("(+-/*^").find(input_string[i - 1]) != std::string::npos)
        ret = 1;
      if ((i + 1) < input_len &&
          std::string(")+-/*^").find(input_string[i + 1]) != std::string::npos)
        ret = 1;
    } else if (input_string[i] == '-' || input_string[i] == '+') {
      if (i + 1 == input_len) ret = 1;
      if (i > 0 &&
          std::string("+-/*^").find(input_string[i - 1]) != std::string::npos)
        ret = 1;
      if ((i + 1) < input_len &&
          std::string(")+-/*^").find(input_string[i + 1]) != std::string::npos)
        ret = 1;
    } else if (input_string[i] == '.' || input_string[i] == ',')
      if (i > 0 && (input_string[i - 1] == '.' || input_string[i - 1] == ','))
        ret = 1;
  }
  if (!input_len || brace_opened || ret)
    ret = 1;
  else {
    for (int i = 0; i < input_len; i++) {
      if (input_string[i] == '-' || input_string[i] == '+') {
        if (i == 0 || (i > 0 && input_string[i - 1] == '(')) {
          input_string.insert(i, "0");
        }
      }
    }
  }
  std::cout << "VALIDATION return: " << ret << "\n";
  return ret;
}

void Model::removeSpaces() {
  std::string::iterator end_pos =
      std::remove(input_string.begin(), input_string.end(), ' ');
  input_string.erase(end_pos, input_string.end());
}

int Model::parsingInput() {
  int ret = 0, priority = 0;
  Model::Type_t en = Model::PLUS;
  size_t len_input_str = input_string.size();
  double number = 0;
  for (size_t i = 0; i < len_input_str && !ret; i++) {
    if (search_num(&i, &number)) {
      lexem.push_back({number, 0, Model::NUMBER});
    } else if (search_substr("sin(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::SIN});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("cos(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::COS});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("tan(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::TAN});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("asin(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::ASIN});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("acos(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::ACOS});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("atan(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::ATAN});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("log(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::LOG});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("ln(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::LN});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("sqrt(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, Model::SQRT});
      lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (search_substr("mod", len_input_str, &i, 1)) {
      lexem.push_back({0, 2, Model::MOD});
    } else if (search_operand(i, &en, &priority)) {
      lexem.push_back({0, priority, en});
    } else if (input_string[i] == '(') {
      if (i + 1 < len_input_str && input_string[i + 1] != ')')
        lexem.push_back({0, -1, Model::BRACE_OPEN});
    } else if (input_string[i] == ')') {
      if ((i + 1 < len_input_str && input_string[i + 1] != '(') ||
          i + 1 == len_input_str)
        lexem.push_back({0, 5, Model::BRACE_CLOSE});
    } else {
      // std::cout << "i= " << i << "\n";
      // std::cout << "char= " << input_string[i] << "\n";
      ret = 1;
      lexem.clear();
    }
  }
  std::cout << "PARSING return: " << ret << "\n";
  return ret;
}

int Model::search_num(size_t* i, double* number) {
  size_t i_old = *i;
  int found = 0;
  std::string buf;
  int j = 0;
  if (input_string[*i] == 'x' && x_exist) {
    found = 1;
    *number = x_value;
  } else if (std::string("0123456789").find(input_string[*i]) !=
             std::string::npos) {
    found = 1;
    for (; found && std::string("0123456789.,").find(input_string[*i]) !=
                        std::string::npos;
         (*i)++) {
      buf.push_back(input_string[*i] == ',' ? '.' : input_string[*i]);
    }
    *number = std::stod(buf);
    if (i_old > 0 && std::string("(+-*/^d").find(input_string[i_old - 1]) ==
                         std::string::npos)
      found = 0;
    *i = found ? (i_old + buf.size() - 1) : i_old;
  }
  return found;
}

int Model::search_substr(std::string search_str, size_t len_input_str,
                         size_t* i, size_t count_char_after) {
  size_t i_old = *i;
  int found = 0;
  size_t len_search_str = search_str.size();
  if (len_input_str - *i >= len_search_str + count_char_after)
    found = 1;  // for sin( must be +2, for mod +1
  if (found) {
    std::string buf;
    for (size_t j = 0; j < len_search_str; (*i)++, j++) {
      buf.push_back(input_string[*i]);
    }
    if (search_str != buf) found = 0;
  }
  if (i_old > 0 &&
      std::string("(+-*/^").find(input_string[i_old - 1]) ==
          std::string::npos &&
      input_string[i_old] != 'm')
    found = 0;
  if (input_string[i_old] == 'm' &&
      (i_old == 0 ||
       (i_old > 0 && std::string("+-/*(^").find(input_string[i_old - 1]) !=
                         std::string::npos)))
    found = 0;
  if (search_str == "mod" &&
      std::string("-(0123456789").find(input_string[*i]) == std::string::npos)
    found = 0;
  *i = found ? (i_old + len_search_str - 1) : i_old;
  return found;
}

int Model::search_operand(size_t i, Model::Type_t* en, int* priority) {
  int found = 0;
  if (std::string("+-^*/").find(input_string[i]) != std::string::npos) {
    found = 1;
    if (input_string[i] == '+') {
      *en = Model::PLUS;
      *priority = 1;
    }
    if (input_string[i] == '-') {
      *en = Model::MINUS;
      *priority = 1;
    }
    if (input_string[i] == '*') {
      *en = Model::MULT;
      *priority = 2;
    }
    if (input_string[i] == '/') {
      *en = Model::DIV;
      *priority = 2;
    }
    if (input_string[i] == '^') {
      *en = Model::POW;
      *priority = 3;
    }
  }
  return found;
}

int Model::rpn_and_calculate() {
  int ret = 0;
  // reverse_stack(&list);
  std::list<std::tuple<double, int, Model::Type_t>> ready_list, support_list,
      temp_support;
  if (lexem.size()) {
    do {
      std::list<std::tuple<double, int, Model::Type_t>> temp_lexeme;
      temp_lexeme.push_back(lexem.front());
      lexem.pop_front();
      if (std::get<2>(temp_lexeme.back()) == Model::BRACE_CLOSE) {
        while (support_list.size() &&
               std::get<2>(support_list.back()) != Model::BRACE_OPEN) {
          temp_support.push_back(support_list.back());
          support_list.pop_back();
          ready_list.push_back(temp_support.back());
          // temp_support.clear();  // del maybe
          temp_support.push_back(support_list.back());
        }
        if (support_list.size()) support_list.pop_back();
      } else if (std::get<2>(temp_lexeme.back()) == Model::BRACE_OPEN) {
        support_list.push_back(temp_lexeme.back());
      } else if (std::get<2>(temp_lexeme.back()) == Model::NUMBER ||
                 std::get<2>(temp_lexeme.back()) == Model::X) {
        ready_list.push_back(temp_lexeme.back());
      } else if (std::get<2>(temp_lexeme.back()) != Model::NUMBER) {
        // temp_support.clear();
        temp_support.push_back(support_list.back());
        for (; temp_support.size() && std::get<1>(temp_support.back()) >=
                                          std::get<1>(temp_lexeme.back());) {
          temp_support.push_back(support_list.back());
          if (support_list.size()) support_list.pop_back();
          ready_list.push_back(temp_support.back());
          // temp_support.clear();
          temp_support.push_back(support_list.back());
        }
        support_list.push_back(temp_lexeme.back());
      }
    } while (lexem.size());
    std::cout << ready_list.size() << " ";
    std::cout << support_list.size() << " ";
    std::cout << temp_support.size() << " ";

    while (support_list.size()) {
      temp_support.push_back(support_list.back());
      support_list.pop_back();
      ready_list.push_back(temp_support.back());
      // temp_support.clear();
      temp_support.push_back(support_list.back());
    }
    std::cout << "\nReady list count: " << ready_list.size() << "\n";
    lexem = ready_list;
    // for (; !lexem.empty(); lexem.pop_front())
    //   std::cout << "lexem:" << std::get<0>(lexem.front()) << "\n";
    // reverse_stack(&ready_list);
    ret = calculate();
  } else
    ret = 1;
  return ret;
}

int Model::calculate() {
  int ret = 0;
  // struct res_stack res;
  int i = 0;
  std::cout << "lexem size: " << lexem.size() << "\n";
  res_stack.reserve(lexem.size());
  // std::list<std::tuple<double, int, Model::Type_t>> current_lexeme;
  if (lexem.size()) {
    do {
      std::cout << "lexem:" << std::get<0>(lexem.front()) << "\n";
      // current_lexeme.push_back(lexem.front());
      if (std::get<2>(lexem.front()) == Model::NUMBER) {
        // std::cout << "Good\n";
        res_stack[i] = std::get<0>(lexem.front());
        i++;
      } else if (is_func_unary(std::get<2>(lexem.front()))) {
        res_stack[i - 1] =
            unary_func_using(std::get<2>(lexem.front()), res_stack[i - 1]);
      } else if (is_func_binary(std::get<2>(lexem.front()))) {
        res_stack[i - 2] = binary_func_using(
            std::get<2>(lexem.front()), res_stack[i - 2], res_stack[i - 1]);
        i--;
      }
      lexem.pop_front();
    } while (lexem.size());
    std::cout << "stack top: " << res_stack[0] << "\n";
    result = res_stack[0];
  } else
    ret = 1;

  return ret;
}

int Model::is_func_unary(Model::Type_t type) {
  int ret = 0;
  if (type == Model::SIN || type == Model::COS || type == Model::TAN ||
      type == Model::ASIN || type == Model::ACOS || type == Model::ATAN ||
      type == Model::LN || type == Model::LOG || type == Model::SQRT)
    ret = 1;
  return ret;
}

double Model::unary_func_using(Model::Type_t type, double val) {
  double result = 0.0;
  if (type == SIN)
    result = std::sin(val);
  else if (type == COS)
    result = std::cos(val);
  else if (type == TAN)
    result = std::tan(val);
  else if (type == ACOS)
    result = std::acos(val);
  else if (type == ASIN)
    result = std::asin(val);
  else if (type == ATAN)
    result = std::atan(val);
  else if (type == LN)
    result = std::log(val);
  else if (type == LOG)
    result = std::log10(val);
  else if (type == SQRT)
    result = std::sqrt(val);
  return result;
}

int Model::is_func_binary(Model::Type_t type) {
  int ret = 0;
  if (type == Model::PLUS || type == Model::MINUS || type == Model::MULT ||
      type == Model::DIV || type == Model::MOD || type == Model::POW)
    ret = 1;
  return ret;
}

double Model::binary_func_using(Model::Type_t type, double val_1,
                                double val_2) {
  double result = 0.0;
  if (type == Model::PLUS)
    result = val_1 + val_2;
  else if (type == Model::MINUS)
    result = val_1 - val_2;
  else if (type == Model::MULT)
    result = val_1 * val_2;
  else if (type == Model::DIV)
    result = val_1 / val_2;
  else if (type == Model::MOD)
    result = std::fmod(val_1, val_2);
  else if (type == Model::POW)
    result = std::pow(val_1, val_2);
  return result;
}

}  // namespace s21