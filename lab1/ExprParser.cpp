#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>


#include "ExprParser.hpp"


using namespace std;

string numbers_re = "\\d*\\.\\d+|\\d+\\.?"; // numbers 1, 1., 1.1, .1
string braces_re = "\\(|\\)";               // braces (, )
string ops_re = "\\+|\\-|\\*|\\/|\\&";         // basic ops +, -, *, /, & (& - special bin op symbol)
string bin_op_re = "\\((.*),(.*)\\)"; // bin op log(1.,2)

ExprParser::ExprParser(string expr, string bin_op_name) {
  if (expr.find("&") != string::npos || expr.find("|") != string::npos) {
    cout << "Expression parsing failed: forbidden symbol & or |";
    raw_expr = "";
    return;
  }

  regex re(numbers_re + "|" + braces_re + "|" + ops_re + "|" + bin_op_name);
  expr = regex_replace(expr, regex("\\s"), "");
  expr = regex_replace(expr, regex(bin_op_name + bin_op_re), "$1&$2");
  expr = regex_replace(expr, re, "|$&|");
  raw_expr = expr;
}

vector<ExprElem> ExprParser::parse() {
  if (raw_expr == "")
    return parsed_expr;

  parsed_expr.clear();
  
  stringstream ss_raw_expr(raw_expr);
  string el_str;
  regex num_regex(numbers_re), br_regex(braces_re), op_regex(ops_re);
  ExprElem prev_el = {EE::INVALID, 0.0};
  int braces_cnt = 0;

  while (getline(ss_raw_expr, el_str, '|')) {
    if (el_str.length() == 0)
      continue;
    ExprElem elem;

    if (regex_match(el_str, num_regex)) {
      if (prev_el.type == EE::NUMBER) {
        cout << "Expression parsing failed: invalid elements order (numbers)!" << endl;
        return {};
      }
      elem.type = EE::NUMBER;
      elem.num = stof(el_str);
    } else if (regex_match(el_str, op_regex)) {
      if (prev_el.type == EE::OP) {
        cout << "Expression parsing failed: invalid elements order (operations)!" << endl;
        return {};
      }
      elem.type = EE::OP;
      elem.symbol = el_str[0];
    } else if (regex_match(el_str, br_regex)) {
      if (el_str == ")" && prev_el.type == EE::BRACE && prev_el.symbol == '('){
        cout << "Expression parsing failed: empty braces!" << endl;
        return {};
      }
      braces_cnt += el_str == "(" ? 1 : -1;
      if (braces_cnt < 0) {
        cout << "Expression parsing failed: missmathced braces!" << endl;
        return {};
      }
      elem.type = EE::BRACE;
      elem.symbol = el_str[0];
    } else {
      cout << "Expression parsing failed: invalid element!" << endl;
      return {};
    }
    prev_el = elem;
    parsed_expr.push_back(elem);
  }
  if (braces_cnt != 0) {
    cout << "Expression parsing failed: missmathced braces!" << endl;
    return {};
  }
  return parsed_expr;
}