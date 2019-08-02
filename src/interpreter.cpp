#include "interpreter.hpp"
#include "math.hpp"
#include <iostream>

Interpreter::Interpreter(std::shared_ptr<INode> ast) :
	ast(ast),
	has_divide(false)
{}

INodePtr Interpreter::visit(INodePtr node)
{
	if (!is_type<OptNode>(node))
		return node;

	auto op_node = std::dynamic_pointer_cast<OptNode>(node);

	auto left = visit(op_node->left);
	if (is_type<ErrorNode>(left))
		return left;
	auto left_node = std::dynamic_pointer_cast<ExprNode>(left);
	
	auto right = visit(op_node->right);
	if (is_type<ErrorNode>(right))
		return right;
	auto right_node = std::dynamic_pointer_cast<ExprNode>(right);

	return operate(op_node, left_node, right_node);
}

INodePtr Interpreter::operate(OptPtr op_node, ExprPtr left, ExprPtr right)
{
	try
	{
		switch (op_node->op)
		{
			case TokenType::PLUS:
				return std::make_shared<ExprNode>(*left + *right);
			case TokenType::MINUS:
				return std::make_shared<ExprNode>(*left - *right);
			case TokenType::MULTIPLY:
				return std::make_shared<ExprNode>(*left * *right);
			case TokenType::DIVIDE:
				if (right->term_map.rbegin()->second > 0)
					has_divide = true;
				return std::make_shared<ExprNode>(*left / *right);
			case TokenType::POWER:
				return std::make_shared<ExprNode>(*left ^ *right);
			case TokenType::EQUAL:
				return equal(left, right);
			default:
				return std::make_shared<ErrorNode>("unsupported operator");
		}
	}
	catch(const std::exception& e)
	{
		return std::make_shared<ErrorNode>(e.what());
	}
	return std::make_shared<ErrorNode>("undefined operator");
}

INodePtr Interpreter::equal(ExprPtr left, ExprPtr right)
{
	auto ret = std::make_shared<ExprNode>(*left - *right);
	if (ret->term_map.rbegin()->first == 0)
	{
		std::cout << "reduced form : " << *ret << " = 0\n";
		return std::make_shared<ErrorNode>("reduced form don't have variable");
	}
	return ret;
}

void Interpreter::solve()
{
	result->clean_map();
	auto end = result->term_map.rbegin();

	if (result->term_map.size() == 1 && end->first == 0)
	{
		solution.push_back(Complex(result->term_map[0], 0));
		return;
	}

	// handle excess and negative degree
	has_excess = result->remove_excess_degree();
	has_neg = result->remove_negative_degree();
	if (has_neg)
	{
		has_divide = true;
		std::cout << "cleaned form : " << *result << " = 0\n";
	}
	else if (has_excess)
	{
		if (result->term_map.size() == 1)
			std::cout << "reduce degree: " << *result << " = 0\n";
		else
			std::cout << "reduce degree: (" << result->var_name << ")(" << *result << ") = 0\n";
	}

	if (end->first == 1)
		solve_one_degree();
	else if (end->first == 2)
		solve_two_degree();
	else
	{
		std::cerr << "cannot handle degree greater than two\n";
		return;
	}
}


void Interpreter::solve_one_degree()
{
	if (has_divide && result->term_map[0] == 0)
		return;
	else if (result->term_map[0] == 0)
		solution.push_back(Complex(0, 0));
	else if (has_excess && !has_divide)
	{
		solution.push_back(Complex(0, 0));
		solution.push_back(Complex(-result->term_map[0] / result->term_map[1], 0));
	}
	else
		solution.push_back(Complex(-result->term_map[0] / result->term_map[1], 0));
}

void Interpreter::solve_two_degree()
{
	get_two_degree_solution();
	if (has_excess)
		solution.push_back(Complex(0, 0));
	if (has_divide)
		solution.remove_if([](Complex & c){ return c.rational == 0 && c.imaginary == 0; });
}


void Interpreter::get_two_degree_solution()
{
	double a = result->term_map[2];
    double b = result->term_map.find(1) != result->term_map.end() ? result->term_map[1] : 0;
    double c = result->term_map.find(0) != result->term_map.end() ? result->term_map[0] : 0;
	double inside = power(b, 2) - (4 * a * c);
	double denominator = 2 * a;
	Complex ret_one;
	Complex ret_two;
	if (inside < 0)
	{
		ret_one.rational = -b / denominator;
		ret_one.imaginary = sqrt(-inside) / denominator;
		ret_two.rational = -b / denominator;
		ret_two.imaginary = -sqrt(-inside) / denominator;
	}
	else
	{
		ret_one.rational = (-b + sqrt(inside)) / denominator;
		ret_two.rational = (-b - sqrt(inside)) / denominator;
	}
	solution.push_back(ret_one);
	if (ret_one.rational - ret_two.rational >= 0.000001 || ret_one.imaginary - ret_two.imaginary >= 0.000001)
		solution.push_back(ret_two);
}

void Interpreter::print_degree()
{
	auto end = result->term_map.rbegin();
	if (has_excess)
		std::cout << "degree       : " << end->first + 1 << "\n";
	else
		std::cout << "degree       : " << end->first << "\n";
}

void Interpreter::print_solution()
{
	if (has_divide)
		std::cout << "limitation   : x != 0\n";
	if (solution.size() == 0)
		std::cout << "no solution\n";
	else
	{
		std::cout << "solution     : " << result->var_name << " = ";
		for (auto it = solution.begin(); it != solution.end(); ++it)
		{
			if (it != solution.begin())
				std::cout << ", ";
			std::cout << *it;
		}
		std::cout << "\n";
	}
}


void Interpreter::interpret()
{
	auto result_inode = visit(ast);
	if (is_type<ExprNode>(result_inode))
	{
		result = std::make_shared<ExprNode>(*std::dynamic_pointer_cast<ExprNode>(result_inode));

		// handle negative coefficient on highest degree
		auto end = result->term_map.rbegin();
		if (end->first != 0 && end->second < 0)
			result = std::make_shared<ExprNode>(*result * ExprNode(-1));

		std::cout << "reduced form : " << *result;
		if (result->term_map.size() != 1 || result->term_map.begin()->first != 0)
			std::cout << " = 0";
		std::cout << "\n";

		solve();
		print_degree();
		print_solution();
	}
	else if (is_type<ErrorNode>(result_inode))
	{
		auto error_node = std::dynamic_pointer_cast<ErrorNode>(result_inode);
		std::cerr << error_node->message << "\n";
	}
}