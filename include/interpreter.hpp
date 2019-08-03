#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include <list>
#include <memory>
#include <string>

class Interpreter
{
    INodePtr ast;
    ExprPtr result;
    bool has_divide;
    bool has_excess_degree;
    bool has_negative_degree;
    std::list<Complex> solution;

    INodePtr visit(INodePtr node);
    INodePtr operate(OptPtr op_node, ExprPtr left, ExprPtr right);
    INodePtr equal(ExprPtr left, ExprPtr right);

    void clean_result();

    void solve();
    void solve_one_degree();
    void solve_two_degree();
    void get_two_degree_solution();

    void print();
    void print_reduced_form();
    void print_degree();
    void print_solution();

public:
    Interpreter(std::shared_ptr<INode>);
    void interpret();

};