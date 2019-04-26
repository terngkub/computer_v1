#include "parser.hpp"

Parser::Parser(Lexer & lexer) :
	lexer(lexer),
	current_token(this->lexer.get_next_token()),
	has_equal(false)
{}

INode * Parser::parse()
{
	return equation();
}

// EQUAL
INode * Parser::equation()
{
	INode * node = expression();

	while (current_token->type == TokenType::EQUAL)
	{
		if (has_equal)
            throw std::runtime_error("has more than one equal sign");
		else
			has_equal = true;
		
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = expression();
		node = new OperationNode(token_type, node, right);
	}
	return node;
}

// PLUS, MINUS
INode * Parser::expression()
{
	INode * node = term();

	while (current_token->type == TokenType::PLUS
			|| current_token->type == TokenType::MINUS)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = term();
		node = new OperationNode(token_type, node, right);
	}
	return node;
}

// MULTIPLY, DIVIDE, MODULO
INode * Parser::term()
{
	INode * node = power();

	while (current_token->type == TokenType::MULTIPLY
			|| current_token->type == TokenType::DIVIDE
			|| current_token->type == TokenType::MODULO)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = power();
		node = new OperationNode(token_type, node, right);
	}
	return node;
}

// POWER
INode * Parser::power()
{
	INode * node = factor();

	while (current_token->type == TokenType::POWER)
	{
		TokenType token_type = current_token->type;
		current_token = lexer.get_next_token();
		INode * right = factor();
		node = new OperationNode(token_type, node, right);
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
		node = new TermNode(current_token->str_value);
		current_token = lexer.get_next_token();
	}
	else if (current_token->type == TokenType::LPAREN)
	{
		current_token = lexer.get_next_token();
		node = expression();
		if (current_token->type != RPAREN) {
			std::cerr << "Unmatch parenthesis";
			exit(1);
		}
		current_token = lexer.get_next_token();
	}
	return node;
}
