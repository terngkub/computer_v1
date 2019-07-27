#include "interpreter.hpp"
#include "math.hpp"
#include <iostream>

Interpreter::Interpreter(std::shared_ptr<INode> ast) :
	ast(ast)
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
				return std::make_shared<ExprNode>(*left / *right);
			case TokenType::POWER:
				return std::make_shared<ExprNode>(*left ^ *right);
			case TokenType::EQUAL:
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
	auto beg = node->term_map.begin();
	auto end = node->term_map.rbegin();

	if (beg->first < 0)
		std::cerr << "cannot handle negative exponent\n";
	else if (end->first == 0)
		std::cout << "solution: " << *node << "\n";
	else if (end->first == 1)
		std::cout << "solution: " << node->var_name << " = " << -node->term_map[0] / node->term_map[1] << "\n";
	else if (end->first == 2)
		solve_polynomial(node);
	else
		std::cerr << "cannot handle exponent greater than two\n";
}

void Interpreter::solve_polynomial(ExprPtr node)
{
	double a = node->term_map[2];
    double b = node->term_map.find(1) != node->term_map.end() ? node->term_map[1] : 0;
    double c = node->term_map.find(0) != node->term_map.end() ? node->term_map[0] : 0;
    double ret_one = (-b + sqrt(power(b, 2) - 4 * a * c)) / (2 * a);
    double ret_two = (-b - sqrt(power(b, 2) - 4 * a * c)) / (2 * a);
	if (ret_one - ret_two < 0.000001)
		std::cout << "solution: " << node->var_name << " = " << ret_one << '\n';
	else
		std::cout << "solution: " << node->var_name << " = " << ret_one << ", " << ret_two << '\n';
}

void Interpreter::interpret()
{
	auto result = visit(ast);
	if (is_type<ExprNode>(result))
	{
		auto result_node = std::dynamic_pointer_cast<ExprNode>(result);
		std::cout << "reduced form: " << *result_node;
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