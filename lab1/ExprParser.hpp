#ifndef EXPRPARSER_HPP
#define EXPRPARSER_HPP

#include <string>
#include <vector>
using namespace std;

struct ExprElem {
  enum {NUMBER, BRACE, OP, INVALID} type;
  union {
    float num;
    char symbol;
  };
};
typedef ExprElem EE;


class ExprParser {
private:
  string raw_expr;
  vector<ExprElem> parsed_expr;

public:
  ExprParser(string expr, string bin_op_name);
  vector<ExprElem> parse();
};

#endif