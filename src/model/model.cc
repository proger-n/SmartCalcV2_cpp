#include "model.h"
namespace s21 {
int Model::head_calc() {
  int ret = 0;
  std::cout << "INPUT: " << input_string << "\n";
  if (!validationInput()) {
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
          add_zero_before_minus(input_string, i);
        }
      }
    }
  }
  return ret;
}

void Model::add_zero_before_minus(std::string input, size_t minus_index) {
  std::string new_input;
  for (size_t i = 0; i < input.size(); i++) {
    if (i == minus_index) {
      new_input.push_back('0');
      new_input.push_back(input[i]);
    } else if (i < minus_index)
      new_input.push_back(input[i]);
    else
      new_input.push_back(input[i]);
  }
  //   new_input[strlen(input) + 1] = '\0';
  //   for (size_t i = 0; i < strlen(input) + 1; i++) input[i] = new_input[i];
  //   input[strlen(input) + 1] = '\0';
  input = new_input;
}

void Model::removeSpaces() {
  std::string::iterator end_pos =
      std::remove(input_string.begin(), input_string.end(), ' ');
  input_string.erase(end_pos, input_string.end());
}

}  // namespace s21