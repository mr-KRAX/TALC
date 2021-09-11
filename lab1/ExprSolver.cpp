#include <list>
#include <map>
#include <stack>
#include <vector>

#include <iostream>

#include "ExprSolver.hpp"

using namespace std;

ExprSolver::ExprSolver(vector<EE> expr, std::function<float(float l, float b, bool* status)> bin_op) {
  this->bin_op = bin_op;
  stack<EE> ops;
  map<char, unsigned int> priority = {{'(', 1}, {')', 1}, {'+', 2}, {'-', 2}, {'*', 3}, {'/', 3}, {'&', 4}};

  for (auto el : expr) {
    if (el.type == EE::NUMBER) {
      RPN_expr.push_back(el);
      continue;
    }

    if (ops.empty())
      ops.push(el);
    else if (el.symbol == '(') {
      ops.push(el);
    } else if (el.symbol == ')') {

      while (ops.size() != 0 && ops.top().symbol != '(') {
        RPN_expr.push_back(ops.top());
        ops.pop();
      }
      ops.pop();
    } else {
      while (ops.size() != 0 && priority[ops.top().symbol] >= priority[el.symbol]) {
        RPN_expr.push_back(ops.top());
        ops.pop();
      }
      ops.push(el);
    }
  }
  while (!ops.empty()) {
    RPN_expr.push_back(ops.top());
    ops.pop();
  }

  cout << "Revers Polish Notation: ";
  for (auto el : RPN_expr) {
    if (el.type == EE::NUMBER)
      cout << el.num << " ";
    else
      cout << el.symbol << " ";
  }
  cout << endl;
}

float ExprSolver::solve() {
  float res;
  while (RPN_expr.size() != 1) {
    for (auto it = RPN_expr.begin(); it != RPN_expr.end(); it++) {
      if (it->type == EE::NUMBER)
        continue;
      EE elem = *it;
      RPN_expr.erase(it--);
      EE num2 = *it;
      RPN_expr.erase(it--);
      EE num1 = *it;

      switch (elem.symbol) {
      case '+':
        res = num1.num + num2.num;
        break;
      case '-':
        res = num1.num - num2.num;
        break;
      case '/':
        res = num1.num / num2.num;
        if (num2.num == 0) {
          RPN_expr.clear();
          cout << "Solving failed: division by zero!"<< endl;
          return 1.0f/0.0f;
        }
        break;
      case '*':
        res = num1.num * num2.num;
        break;
      case '&':
        bool status; 
        res = bin_op(num1.num, num2.num, &status);
        if (!status){
          RPN_expr.clear();
          cout << "Solving failed: special operation error!" <<  endl;
          return 1.0f/0.0f;
        }
        break;
      default:
        break;
      }

      it->num = res;
    }
  }

  return RPN_expr.front().num;
}