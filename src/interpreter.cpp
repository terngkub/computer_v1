#include "interpreter.hpp"
#include "math.hpp"
#include <iostream>
#include <sstream>

Interpreter::Interpreter(std::shared_ptr<INode> ast) :
	ast(ast),
	has_divide(false),
	has_equal(false)
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
				has_equal = true;
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

void Interpreter::clean_result()
{
	result->clean_map();

	// handle negative coefficient (-x + 1 => x - 1)
	auto end = result->term_map.rbegin();
	if (end->first != 0 && end->second < 0)
		result = std::make_shared<ExprNode>(*result * ExprNode(-1));

	// handle excess degree (x^3 + x^2 => (x)(x+1))
	has_excess_degree = result->remove_excess_degree();

	// handle negative degree (x^-2 + x^-1 => x + 1 && x != 0)
	has_negative_degree = result->remove_negative_degree();
	if (has_negative_degree)
		has_divide = true;
}

void Interpreter::solve()
{
	auto end = result->term_map.rbegin();

	if (result->term_map.size() == 1 && end->first == 0)
		solution.push_back(Complex(result->term_map[0], 0));
	else if (end->first == 1)
		solve_one_degree();
	else if (end->first == 2)
		solve_two_degree();
}

void Interpreter::solve_one_degree()
{
	if (has_divide && result->term_map.find(0) == result->term_map.end())
		return;
	else if (result->term_map[0] == 0)
		solution.push_back(Complex(0, 0));
	else if (has_excess_degree && !has_divide)
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
	if (has_excess_degree)
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

Output Interpreter::get_output()
{
	Output output;
	if (result->var_name != "" && result->term_map.find(0) != result->term_map.end() && result->term_map[0] == 0)
	{
		output.reduced_form = "0 = 0";
		output.has_limit = has_divide;
		output.solution = result->var_name + " can be any number";
	}
	else if (result->var_name == "" && has_equal)
	{
		output.error = "need variable in equation";
	}
	else
	{
		output.reduced_form = get_reduced_form();
		output.degree = get_degree();
		output.has_limit = has_divide;
		output.solution = get_solution();
	}
	return output;
}

std::string Interpreter::get_reduced_form()
{
	std::stringstream ss;

	if (has_excess_degree && result->term_map.size() != 1)
		ss << "(" << result->var_name << ")(" << *result << ") = 0";
	else
	{
		ss << *result;
		if (result->term_map.size() != 1 || result->term_map.begin()->first != 0)
			ss << " = 0";
	}
	return ss.str();
}

int Interpreter::get_degree()
{
	auto end = result->term_map.rbegin()->first;
	auto degree = has_excess_degree ? end + 1 : end;
	return degree;
}

std::string Interpreter::get_solution()
{
	std::stringstream ss;

	if (result->term_map.rbegin()->first > 2)
		ss << "cannot handle degree greater than two";
	else if (solution.size() == 0)
		ss << "no solution";
	else if (result->term_map.size() == 1 && result->term_map.rbegin()->first == 0)
		ss << *result;
	else
	{
		ss << result->var_name << " = ";
		for (auto it = solution.begin(); it != solution.end(); ++it)
		{
			if (it != solution.begin())
				ss << ", ";
			ss << *it;
		}
	}
	return ss.str();
}

Output Interpreter::interpret()
{
	auto result_inode = visit(ast);
	if (is_type<ExprNode>(result_inode))
	{
		result = std::make_shared<ExprNode>(*std::dynamic_pointer_cast<ExprNode>(result_inode));
		clean_result();
		solve();
		return get_output();
	}
	else if (is_type<ErrorNode>(result_inode))
	{
		auto error_node = std::dynamic_pointer_cast<ErrorNode>(result_inode);
		return Output(error_node->message);
	}
	return Output("invalid node type from Interpret::visit()");
}