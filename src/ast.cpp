#include "ast.hpp"

TermNode::TermNode(std::string name) :
	name(name),
	coef(1),
	power(1)
{}

TermNode::TermNode(double coef) :
	name(""),
	coef(coef),
	power(0)
{}

TermNode::TermNode(double coef, int power) :
	name(""),
	coef(coef),
	power(power)
{}

void TermNode::print()
{
	if (power == 0)
		std::cout << coef;
	else
	{
		if (coef > 1)
			std::cout << coef << " * ";
		if (power == 1)
			std::cout << name;
		else
			std::cout << name << " ^ " << power;
	}
}

OperationNode::OperationNode(enum TokenType op, INode * left, INode * right) :
	op(op),
	left(left),
	right(right)
{}

void OperationNode::print()
{
	std::cout << "(";
	left->print();
	std::cout << op;
	right->print();
	std::cout << ")";
}