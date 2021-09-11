#ifndef EXPRSOLVER_HPP
#define EXPRSOLVER_HPP

#include <list>
#include <functional>

#include "ExprParser.hpp"


class ExprSolver {
private:
  list<EE> RPN_expr; //reverse polish notation;
  std::function<float(float l, float b, bool* status)> bin_op;
public:
  ExprSolver(vector<EE> expr, std::function<float(float l, float b, bool* status)> bin_op);
  float solve();
};

#endif