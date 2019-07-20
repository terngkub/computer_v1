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

OperationNode::OperationNode(enum TokenType op, std::shared_ptr<INode> left, std::shared_ptr<INode> right) :
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

ErrorNode::ErrorNode(std::string message) :
	message(message)
{}

/*
 * Expression Node
 */

ExpressionNode::ExpressionNode(double nb) :
	var_name("")
{
	term_map[0] = nb;
}

ExpressionNode::ExpressionNode(std::string var_name) :
	var_name(var_name)
{
	term_map[1] = 1;
}

// coeff * var_name ^ power
void ExpressionNode::print() const
{
	for (auto it = term_map.rbegin(); it != term_map.rend(); ++it)
	{
		// plus/minus
		if (it != term_map.rbegin())
		{
			if (it->first < 0)
				std::cout << " - ";
			else
				std::cout << " + ";
		}

		// coeff
		std::cout << it->second;

		// variable and power
		if (it->first != 0)
		{
			std::cout << "*" << var_name ;
			if (it->first != 1)
				std::cout << "^" << it->first;
		}
	}
}

bool ExpressionNode::contain_variable() const
{
    if (term_map.size() > 1)
		return true;
    if (term_map.size() == 1 && term_map.find(0) == term_map.end())
	{

        return true;
	}
	return false;
}

