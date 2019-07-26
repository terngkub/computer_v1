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
	return std::make_shared<ErrorNode>("undefined operator");
}

void Interpreter::interpret()
{
	auto result = visit(ast);
	if (is_type<ExprNode>(result))
	{
		auto result_node = std::dynamic_pointer_cast<ExprNode>(result);
		std::cout << *result_node << "\n";
	}
}