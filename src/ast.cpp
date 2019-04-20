#include "ast.hpp"

TermNode::TermNode(double coef) :
	coef(coef),
	power(0)
{}

void TermNode::print()
{
	std::cout << coef << '\n';
}

OperationNode::OperationNode(enum TokenType op, INode * left, INode * right) :
	op(op),
	left(left),
	right(right)
{}

void OperationNode::print()
{
	left->print();
	std::cout << op << '\n';
	right->print();
}