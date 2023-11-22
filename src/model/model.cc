#include "model.h"
namespace s21 {
int Model::head_calc() {
  int ret = 0;
  std::cout << "INPUT: " << input_string << "\n";
  if (!validationInput() && !parsingInput()) {
    ret = 1;
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
  return ret;
}

void Model::removeSpaces() {
  std::string::iterator end_pos =
      std::remove(input_string.begin(), input_string.end(), ' ');
  input_string.erase(end_pos, input_string.end());
}

int Model::parsingInput() {
  int ret = 0, en = 0, priority = 0;
  size_t len_input_str = input_string.size();
  double number = 0;
  for (size_t i = 0; i < len_input_str && !ret; i++) {
    if (search_num(&i, &number)) {
      lexem.push_back({number, 0, s21::Model::NUMBER});
    } else if (search_substr("sin(", len_input_str, &i, 2)) {
      lexem.push_back({0, 4, s21::Model::SIN});
      lexem.push_back({0, -1, s21::Model::BRACE_OPEN});
    }
    // else if (search_substr(input, "cos(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, COS);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "tan(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, TAN);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "asin(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, ASIN);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "acos(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, ACOS);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "atan(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, ATAN);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "log(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, LOG);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "ln(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, LN);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "sqrt(", len_input_str, &i, 2)) {
    //   push(list, 0, 4, SQRT);
    //   push(list, 0, -1, BRACE_OPEN);
    // } else if (search_substr(input, "mod", len_input_str, &i, 1)) {
    //   push(list, 0, 2, MOD);
    // } else if (search_operand(input, i, &en, &priority)) {
    //   push(list, 0, priority, en);
    // } else if (input[i] == '(') {
    //   if (i + 1 < len_input_str && input[i + 1] != ')')
    //     push(list, 0, -1, BRACE_OPEN);
    // } else if (input[i] == ')') {
    //   if ((i + 1 < len_input_str && input[i + 1] != '(') ||
    //       i + 1 == len_input_str)
    //     push(list, 0, 5, BRACE_CLOSE);
    // } else {
    //   ret = 1;
    //   delete_stack(list);
    // }
  }
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
    for (; found && std::string("0123456789").find(input_string[*i]) !=
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

}  // namespace s21