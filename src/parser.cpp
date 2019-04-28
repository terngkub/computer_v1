#include "parser.hpp"

Parser::Parser(Lexer & lexer) :
	lexer(lexer),
	current_token(this->lexer.get_next_token()),
	var_name(""),
	has_equal(false)
{}

INode * Parser::parse()
{
	INode * ast = equation();
	if (dynamic_cast<ErrorNode *>(ast))
		return ast;
	if (current_token->type != TokenType::END)
		return new ErrorNode("not ending");
	return ast;
}

// EQUAL
INode * Parser::equation()
{
	INode * node = expression();

	while (dynamic_cast<ErrorNode *>(node) == nullptr
			&&current_token->type == TokenType::EQUAL)
	{
		if (has_equal)
			return new ErrorNode("statement has more than one equal sign");
		else
			has_equal = true;
		
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = expression();
		ErrorNode * err = dynamic_cast<ErrorNode *>(right);
		if (err != nullptr)
			return new ErrorNode(err->message);
		else
			node = new OperationNode(token_type, node, right);
	}
	return node;
}

// PLUS, MINUS
INode * Parser::expression()
{
	INode * node = term();

	while (dynamic_cast<ErrorNode *>(node) == nullptr
			&& (current_token->type == TokenType::PLUS
			|| current_token->type == TokenType::MINUS))
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = term();
		auto err = dynamic_cast<ErrorNode *>(right);
		if (err != nullptr)
			return new ErrorNode(err->message);
		else
			node = new OperationNode(token_type, node, right);
	}
	return node;
}

// MULTIPLY, DIVIDE, MODULO
INode * Parser::term()
{
	INode * node = power();

	while (dynamic_cast<ErrorNode *>(node) == nullptr
			&& (current_token->type == TokenType::MULTIPLY
			|| current_token->type == TokenType::DIVIDE
			|| current_token->type == TokenType::MODULO))
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = power();
		auto err = dynamic_cast<ErrorNode *>(right);
		if (err != nullptr)
			return new ErrorNode(err->message);
		else
			node = new OperationNode(token_type, node, right);
	}
	return node;
}

// POWER
INode * Parser::power()
{
	INode * node = factor();

	while (dynamic_cast<ErrorNode *>(node) == nullptr
			&& current_token->type == TokenType::POWER)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = factor();
		auto err = dynamic_cast<ErrorNode *>(right);
		if (err != nullptr)
			return new ErrorNode(err->message);
		else
		{
			// check combination
			auto right_node = dynamic_cast<TermNode *>(right);
			if (right_node != nullptr && right_node->name != "")
				return new ErrorNode("degree can't be variable");

			node = new OperationNode(token_type, node, right);
		}
	}
	return node;
}

// NUMBER, VARIABLE
INode * Parser::factor()
{
	INode * node = nullptr;
	if (current_token->type == TokenType::NUMBER)
	{
		node = new TermNode(current_token->num_value);
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::VARIABLE)
	{
		auto name = current_token->str_value;
		if (var_name == "")
			var_name = name;
		else if (var_name != name)
			return new ErrorNode("too much variable");
		node = new TermNode(current_token->str_value);
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::LPAREN)
	{
		current_token = lexer.get_next_token();
		node = expression();

		if (current_token->type != RPAREN)
			node = new ErrorNode("parenthesis not matching");
		else
			current_token = lexer.get_next_token();
	}
	else
		node = new ErrorNode("invalid type");
	return node;
}
