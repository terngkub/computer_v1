#pragma once
#include "ast.hpp"
#include <string>


class Interpreter
{
    INode * ast;
    std::string name;
    std::map<int, double> term_map;

    INode * visit(INode * node);
    void update_term_map(TermNode * node, bool is_plus);
    void print_map();

public:
    Interpreter(INode *);
    void interpret();

};