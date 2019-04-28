#pragma once
#include "ast.hpp"
#include <string>
#include <map>


class Interpreter
{
    INode * ast;
    std::string var_name;
    std::map<int, double> term_map;

    INode * visit(INode * node);
    void update_term_map(TermNode * node, bool is_plus);
    void print_map();
    void solve();
    bool is_valid_equation();
    void solve_polynomial_equation();
    void solve_linear_equation();

public:
    Interpreter(INode *);
    void interpret();

};