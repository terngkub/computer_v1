#include "parser.hpp"

Parser::Parser(Lexer & lexer) :
	lexer(lexer),
	current_token(this->lexer.get_next_token()),
	var_name(""),
	has_equal(false)
{}

std::shared_ptr<INode> Parser::parse()
{
	std::shared_ptr<INode> ast = equation();
	if (is_error(ast))
		return ast;
	if (current_token->type != TokenType::END)
		return error("not ending");
	return ast;
}

// ERROR
std::shared_ptr<INode> Parser::error(std::string error_message)
{
	return std::dynamic_pointer_cast<INode>(std::make_shared<ErrorNode>(error_message));
}

bool Parser::is_error(std::shared_ptr<INode> node)
{
	return (std::dynamic_pointer_cast<ErrorNode>(node) != nullptr) ? true : false;
}

// EQUAL
std::shared_ptr<INode> Parser::equation()
{
	std::shared_ptr<INode> node = expression();

	while (!is_error(node) && current_token->type == TokenType::EQUAL)
	{
		if (has_equal)
			return error("statement has more than one equal sign");
		else
			has_equal = true;
		
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();

		std::shared_ptr<INode> right = expression();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OperationNode>(token_type, node, right);
	}
	return node;
}

// PLUS, MINUS
std::shared_ptr<INode> Parser::expression()
{
	std::shared_ptr<INode> node = term();

	while (!is_error(node)
			&& (current_token->type == TokenType::PLUS
			|| current_token->type == TokenType::MINUS))
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		std::shared_ptr<INode> right = term();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OperationNode>(token_type, node, right);
	}
	return node;
}

// MULTIPLY, DIVIDE, MODULO
std::shared_ptr<INode> Parser::term()
{
	std::shared_ptr<INode> node = power();

	while (!is_error(node)
			&& (current_token->type == TokenType::MULTIPLY
			|| current_token->type == TokenType::DIVIDE
			|| current_token->type == TokenType::MODULO))
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		std::shared_ptr<INode> right = power();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OperationNode>(token_type, node, right);
	}
	return node;
}

// POWER
std::shared_ptr<INode> Parser::power()
{
	std::shared_ptr<INode> node = factor();

	while (!is_error(node)
			&& current_token->type == TokenType::POWER)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		std::shared_ptr<INode> right = factor();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
		{
			// check combination
			auto right_node = std::dynamic_pointer_cast<TermNode>(right);
			if (right_node != nullptr && right_node->name != "")
				return error("degree can't be variable");

			node = std::make_shared<OperationNode>(token_type, node, right);
		}
	}
	return node;
}

// NUMBER, VARIABLE
std::shared_ptr<INode> Parser::factor()
{
	std::shared_ptr<INode> node = nullptr;
	if (current_token->type == TokenType::NUMBER)
	{
		node = std::make_shared<TermNode>(current_token->num_value);
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::VARIABLE)
	{
		auto name = current_token->str_value;
		if (var_name == "")
			var_name = name;
		else if (var_name != name)
			return error("too much variable");
		node = std::make_shared<TermNode>(current_token->str_value);
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::LPAREN)
	{
		current_token = lexer.get_next_token();
		node = expression();

		if (current_token->type != RPAREN)
			node = error("parenthesis not matching");
		else
			current_token = lexer.get_next_token();
	}
	else
		node = error("invalid type");
	return node;
}
