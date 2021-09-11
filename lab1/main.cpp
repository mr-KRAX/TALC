#include <iostream>

#include "ExprParser.hpp"
#include "ExprSolver.hpp"
#include <cmath>

using namespace std;


int main() {
  // string expr = "(5.0+6  )*7+log(1,.2  ) + .05 - 10.10";2
  // string expr = "(1+2)*(3+4) - 5.0 + log(2,8)";
  string expr;
  cout << "Enter your expression: ";
  std::getline(std::cin, expr);

  auto parsed_expr = ExprParser(expr, "log").parse();
  if (parsed_expr.size() == 0)
    return 0;
  
  cout << "Parsed expr: ";
  for (auto el : parsed_expr) {
    if (el.type == EE::NUMBER)
      cout << el.num << " ";
    else 
      cout << el.symbol << " ";
  }
  cout << endl;

  auto bin_op = [](float l, float r, bool *status) {
    if (l <= 0 || l == 1 || r <= 0) {
      *status = false;
      return 0.0f;
    }
    *status = true;
    return log(r) / log(l);
  };

  float ans = ExprSolver(parsed_expr, bin_op).solve();
  if (ans != 1.0f/0.0f)
    cout << "Answer: " << ans << endl;
  return 0;
}