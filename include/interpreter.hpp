#pragma once
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <memory>

class Interpreter
{
    std::shared_ptr<INode> ast;

    std::shared_ptr<INode> visit(std::shared_ptr<INode> node);
    void update_term_map(std::shared_ptr<TermNode> node, bool is_plus);
    void print_map();
    void solve();
    bool is_valid_equation();
    void solve_polynomial_equation();
    void solve_linear_equation();

public:
    Interpreter(std::shared_ptr<INode>);
    void interpret();

};