#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include <list>
#include <memory>
#include <string>

class Interpreter
{
    std::shared_ptr<INode> ast;
    bool has_divide;

    INodePtr visit(INodePtr node);
    INodePtr operate(OptPtr op_node, ExprPtr left, ExprPtr right);
    void solve(ExprPtr node);
    std::list<Complex> solve_polynomial(ExprPtr node);

public:
    Interpreter(std::shared_ptr<INode>);
    void interpret();

};