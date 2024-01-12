#include "model.h"

#include <algorithm>
#include <cmath>

namespace s21 {

void Model::SetInput(std::string input) { input_string_ = input; }

void Model::SetX(double x_val) {
  x_exist_ = 1;
  x_value_ = x_val;
}

std::string Model::GetResult() {
  std::string ret;
  if ((!ValidationInput() && !ParsingInput()) || !input_string_.size()) {
    if (input_string_.size() && RPNAndCalculate())
      ret = "ERROR";
    else
      ret = std::to_string(result_);
  } else {
    ret = "ERROR";
  }
  return ret;
}

int Model::ValidationInput() {
  RemoveSpaces();
  int ret = 0;
  int brace_opened = 0;
  std::string available_values = "0123456789.,x*/+-^()mdcosinltaqrtg";
  int input_len = input_string_.size();
  for (int i = 0; i < input_len && brace_opened >= 0; i++) {
    if (available_values.find(input_string_[i]) == std::string::npos) {
      ret = 1;
    } else if (input_string_[i] == '(') {
      brace_opened += 1;
    } else if (input_string_[i] == ')') {
      brace_opened -= 1;
      if (i > 0 &&
          std::string("+-/*(^").find(input_string_[i - 1]) != std::string::npos)
        brace_opened = -1;
    } else if (std::string("/*^").find(input_string_[i]) != std::string::npos) {
      if (i + 1 == input_len) ret = 1;
      if (i > 0 &&
          std::string("(+-/*^").find(input_string_[i - 1]) != std::string::npos)
        ret = 1;
      if ((i + 1) < input_len &&
          std::string(")+-/*^").find(input_string_[i + 1]) != std::string::npos)
        ret = 1;
    } else if (input_string_[i] == '-' || input_string_[i] == '+') {
      if (i + 1 == input_len) ret = 1;
      if (i > 0 &&
          std::string("+-/*^").find(input_string_[i - 1]) != std::string::npos)
        ret = 1;
      if ((i + 1) < input_len &&
          std::string(")+-/*^").find(input_string_[i + 1]) != std::string::npos)
        ret = 1;
    } else if (input_string_[i] == '.' || input_string_[i] == ',')
      if (i > 0 && (input_string_[i - 1] == '.' || input_string_[i - 1] == ','))
        ret = 1;
  }
  if (!input_len || brace_opened || ret)
    ret = 1;
  else {
    for (int i = 0; i < input_len; i++) {
      if (input_string_[i] == '-' || input_string_[i] == '+') {
        if (i == 0 || (i > 0 && input_string_[i - 1] == '(')) {
          input_string_.insert(i, "0");
        }
      }
    }
  }
  return ret;
}

void Model::RemoveSpaces() {
  std::string::iterator end_pos =
      std::remove(input_string_.begin(), input_string_.end(), ' ');
  input_string_.erase(end_pos, input_string_.end());
}

int Model::ParsingInput() {
  int ret = 0, priority = 0;
  TypeT en = kPlus;
  std::size_t len_input_str = input_string_.size();
  double number = 0;
  for (std::size_t i = 0; i < len_input_str && !ret; i++) {
    if (SearchNum(&i, &number)) {
      lexem_.push_back({number, 0, kNumber});
    } else if (SearchSubstr("sin(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kSin});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("cos(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kCos});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("tan(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kTan});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("asin(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kAsin});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("acos(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kAcos});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("atan(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kAtan});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("log(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kLog});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("ln(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kLn});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("sqrt(", len_input_str, &i, 2)) {
      lexem_.push_back({0, 4, kSqrt});
      lexem_.push_back({0, -1, kBraceOpen});
    } else if (SearchSubstr("mod", len_input_str, &i, 1)) {
      lexem_.push_back({0, 2, kMod});
    } else if (SearchOperand(i, &en, &priority)) {
      lexem_.push_back({0, priority, en});
    } else if (input_string_[i] == '(') {
      if (i + 1 < len_input_str && input_string_[i + 1] != ')')
        lexem_.push_back({0, -1, kBraceOpen});
    } else if (input_string_[i] == ')') {
      if ((i + 1 < len_input_str && input_string_[i + 1] != '(') ||
          i + 1 == len_input_str)
        lexem_.push_back({0, 5, kBraceClose});
    } else {
      ret = 1;
      lexem_.clear();
    }
  }
  return ret;
}

int Model::SearchNum(std::size_t* i, double* number) {
  std::size_t i_old = *i;
  int found = 0;
  std::string buf;
  if (input_string_[*i] == 'x' && x_exist_) {
    found = 1;
    *number = x_value_;
  } else if (std::string("0123456789").find(input_string_[*i]) !=
             std::string::npos) {
    found = 1;
    for (; found && std::string("0123456789.,").find(input_string_[*i]) !=
                        std::string::npos;
         (*i)++) {
      buf.push_back(input_string_[*i] == ',' ? '.' : input_string_[*i]);
    }
    *number = std::stod(buf);
    if (i_old > 0 && std::string("(+-*/^d").find(input_string_[i_old - 1]) ==
                         std::string::npos)
      found = 0;
    *i = found ? (i_old + buf.size() - 1) : i_old;
  }
  return found;
}

int Model::SearchSubstr(std::string search_str, std::size_t len_input_str,
                        std::size_t* i, std::size_t count_char_after) {
  std::size_t i_old = *i;
  int found = 0;
  std::size_t len_search_str = search_str.size();
  if (len_input_str - *i >= len_search_str + count_char_after) found = 1;
  if (found) {
    std::string buf;
    for (std::size_t j = 0; j < len_search_str; (*i)++, j++) {
      buf.push_back(input_string_[*i]);
    }
    if (search_str != buf) found = 0;
  }
  if (i_old > 0 &&
      std::string("(+-*/^").find(input_string_[i_old - 1]) ==
          std::string::npos &&
      input_string_[i_old] != 'm')
    found = 0;
  if (input_string_[i_old] == 'm' &&
      (i_old == 0 ||
       (i_old > 0 && std::string("+-/*(^").find(input_string_[i_old - 1]) !=
                         std::string::npos)))
    found = 0;
  if (search_str == "mod" &&
      std::string("-(0123456789").find(input_string_[*i]) == std::string::npos)
    found = 0;
  *i = found ? (i_old + len_search_str - 1) : i_old;
  return found;
}

int Model::SearchOperand(std::size_t i, TypeT* en, int* priority) {
  int found = 0;
  if (std::string("+-^*/").find(input_string_[i]) != std::string::npos) {
    found = 1;
    if (input_string_[i] == '+') {
      *en = kPlus;
      *priority = 1;
    }
    if (input_string_[i] == '-') {
      *en = kMinus;
      *priority = 1;
    }
    if (input_string_[i] == '*') {
      *en = kMult;
      *priority = 2;
    }
    if (input_string_[i] == '/') {
      *en = kDiv;
      *priority = 2;
    }
    if (input_string_[i] == '^') {
      *en = kPow;
      *priority = 3;
    }
  }
  return found;
}

int Model::RPNAndCalculate() {
  int ret = 0;
  std::list<std::tuple<double, int, TypeT>> ready_list, support_list,
      temp_support;
  if (lexem_.size()) {
    do {
      std::list<std::tuple<double, int, TypeT>> temp_lexeme;
      temp_lexeme.push_back(lexem_.front());
      lexem_.pop_front();
      if (std::get<2>(temp_lexeme.back()) == kBraceClose) {
        while (support_list.size() &&
               std::get<2>(support_list.back()) != kBraceOpen) {
          temp_support.push_back(support_list.back());
          support_list.pop_back();
          ready_list.push_back(temp_support.back());
          temp_support.push_back(support_list.back());
        }
        if (support_list.size()) support_list.pop_back();
      } else if (std::get<2>(temp_lexeme.back()) == kBraceOpen) {
        support_list.push_back(temp_lexeme.back());
      } else if (std::get<2>(temp_lexeme.back()) == kNumber ||
                 std::get<2>(temp_lexeme.back()) == kX) {
        ready_list.push_back(temp_lexeme.back());
      } else if (std::get<2>(temp_lexeme.back()) != kNumber) {
        temp_support.push_back(support_list.back());
        for (; temp_support.size() && std::get<1>(temp_support.back()) >=
                                          std::get<1>(temp_lexeme.back());) {
          temp_support.push_back(support_list.back());
          if (support_list.size()) support_list.pop_back();
          ready_list.push_back(temp_support.back());
          temp_support.push_back(support_list.back());
        }
        support_list.push_back(temp_lexeme.back());
      }
    } while (lexem_.size());

    while (support_list.size()) {
      temp_support.push_back(support_list.back());
      support_list.pop_back();
      ready_list.push_back(temp_support.back());
      temp_support.push_back(support_list.back());
    }
    lexem_ = ready_list;
    ret = Calculate();
  } else
    ret = 1;
  return ret;
}

int Model::Calculate() {
  int ret = 0;
  int i = 0;
  res_stack_.reserve(lexem_.size());
  if (lexem_.size()) {
    do {
      if (std::get<2>(lexem_.front()) == kNumber) {
        res_stack_[i] = std::get<0>(lexem_.front());
        i++;
      } else if (IsFuncUnary(std::get<2>(lexem_.front()))) {
        res_stack_[i - 1] =
            UnaryFuncUsing(std::get<2>(lexem_.front()), res_stack_[i - 1]);
      } else if (IsFuncBinary(std::get<2>(lexem_.front()))) {
        res_stack_[i - 2] = BinaryFuncUsing(
            std::get<2>(lexem_.front()), res_stack_[i - 2], res_stack_[i - 1]);
        i--;
      }
      lexem_.pop_front();
    } while (lexem_.size());
    result_ = res_stack_[0];
  } else
    ret = 1;

  return ret;
}

int Model::IsFuncUnary(TypeT type) {
  int ret = 0;
  if (type == kSin || type == kCos || type == kTan || type == kAsin ||
      type == kAcos || type == kAtan || type == kLn || type == kLog ||
      type == kSqrt)
    ret = 1;
  return ret;
}

double Model::UnaryFuncUsing(TypeT type, double val) {
  double result_ = 0.0;
  if (type == kSin)
    result_ = std::sin(val);
  else if (type == kCos)
    result_ = std::cos(val);
  else if (type == kTan)
    result_ = std::tan(val);
  else if (type == kAcos)
    result_ = std::acos(val);
  else if (type == kAsin)
    result_ = std::asin(val);
  else if (type == kAtan)
    result_ = std::atan(val);
  else if (type == kLn)
    result_ = std::log(val);
  else if (type == kLog)
    result_ = std::log10(val);
  else if (type == kSqrt)
    result_ = std::sqrt(val);
  return result_;
}

int Model::IsFuncBinary(TypeT type) {
  int ret = 0;
  if (type == kPlus || type == kMinus || type == kMult || type == kDiv ||
      type == kMod || type == kPow)
    ret = 1;
  return ret;
}

double Model::BinaryFuncUsing(TypeT type, double val_1, double val_2) {
  double result_ = 0.0;
  if (type == kPlus)
    result_ = val_1 + val_2;
  else if (type == kMinus)
    result_ = val_1 - val_2;
  else if (type == kMult)
    result_ = val_1 * val_2;
  else if (type == kDiv)
    result_ = val_1 / val_2;
  else if (type == kMod)
    result_ = std::fmod(val_1, val_2);
  else if (type == kPow)
    result_ = std::pow(val_1, val_2);
  return result_;
}

}  // namespace s21
