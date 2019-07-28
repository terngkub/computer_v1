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
				// if left and right don't have variable -> error
				if (left->term_map.size() == 1 && left->term_map.begin()->first == 0
						&& right->term_map.size() == 1 && right->term_map.begin()->first == 0)
					return std::make_shared<ErrorNode>("invalid input: equal sign with no variable");
				return std::make_shared<ExprNode>(*left - *right);
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

void Interpreter::solve(ExprPtr node)
{
	node->clean_map();
	auto end = node->term_map.rbegin();

	if (node->term_map.size() == 1 && end->first == 0)
	{
		std::cout << "solution     : " << *node << "\n";
		return;
	}

	bool has_excess = node->remove_excess_degree();
	bool has_neg = node->remove_negative_degree();
	if (has_neg)
	{
		has_divide = true;
		std::cout << "shifted form : " << *node << " = 0\n";
	}
	else if (has_excess)
		std::cout << "shifted form : (" << node->var_name << ")(" << *node << ") = 0\n";
	if (has_divide)
		std::cout << "limitation   : x != 0\n";

	if (end->first == 1)
	{
		if (has_divide && node->term_map[0] == 0)
			std::cout << "no solution\n";
		else if (node->term_map[0] == 0)
			std::cout << "solution     : " << node->var_name << " = 0\n";
		else if (has_excess && !has_divide)
			std::cout << "solution     : " << node->var_name << " = 0, " << -node->term_map[0] << "\n";
		else
			std::cout << "solution     : " << node->var_name << " = " << -node->term_map[0] << "\n";
	}
	else if (end->first == 2)
	{
		auto solution = solve_polynomial(node);
		if (has_excess)
			solution.push_back(Complex(0, 0));
		if (has_divide)
			solution.remove_if([](Complex & c){ return c.rational == 0 && c.imaginary == 0; });
		if (solution.size() == 0)
			std::cout << "no solution\n";
		else
		{
			std::cout << "solution     : " << node->var_name << " = ";
			for (auto it = solution.begin(); it != solution.end(); ++it)
			{
				if (it != solution.begin())
					std::cout << ", ";
				std::cout << *it;
			}
			std::cout << "\n";
		}
	}
	else
	{
		std::cerr << "cannot handle degree greater than two\n";
		return;
	}
}

std::list<Complex> Interpreter::solve_polynomial(ExprPtr node)
{
	double a = node->term_map[2];
    double b = node->term_map.find(1) != node->term_map.end() ? node->term_map[1] : 0;
    double c = node->term_map.find(0) != node->term_map.end() ? node->term_map[0] : 0;
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
	std::list<Complex> solution;
	solution.push_back(ret_one);
	if (ret_one.rational - ret_two.rational >= 0.000001 || ret_one.imaginary - ret_two.imaginary >= 0.000001)
		solution.push_back(ret_two);
	return solution;
}

void Interpreter::interpret()
{
	auto result = visit(ast);
	if (is_type<ExprNode>(result))
	{
		auto result_node = std::dynamic_pointer_cast<ExprNode>(result);

		// handle negative coefficient on highest degree
		auto end = result_node->term_map.rbegin();
		if (end->first != 0 && end->second < 0)
			result_node = std::make_shared<ExprNode>(*result_node * ExprNode(-1));

		std::cout << "reduced form : " << *result_node;
		if (result_node->term_map.size() != 1 || result_node->term_map.begin()->first != 0)
			std::cout << " = 0";
		std::cout << "\n";

		solve(result_node);
	}
	else if (is_type<ErrorNode>(result))
	{
		auto error_node = std::dynamic_pointer_cast<ErrorNode>(result);
		std::cerr << error_node->message << "\n";
	}
}