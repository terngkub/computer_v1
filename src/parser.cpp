#include "parser.hpp"

Parser::Parser(Lexer & lexer) :
	lexer(lexer),
	current_token(this->lexer.get_next_token()),
	var_name(""),
	has_equal(false)
{}

INodePtr Parser::parse()
{
	INodePtr ast = equation();
	if (is_type<ErrorNode>(ast))
		return ast;
	if (current_token->type != TokenType::END)
		return error("not ending");
	return ast;
}

void Parser::get_next_token()
{
	prev_token = current_token;
	current_token = lexer.get_next_token();
}

bool Parser::is_minus()
{
	return current_token->type == TokenType::MINUS
			&& (prev_token->type == TokenType::NUMBER
				|| prev_token->type == TokenType::VARIABLE
				|| prev_token->type == TokenType::RPAREN);
}

// ERROR
INodePtr Parser::error(std::string error_message)
{
	return std::dynamic_pointer_cast<INode>(std::make_shared<ErrorNode>(error_message));
}

// EQUAL
INodePtr Parser::equation()
{
	INodePtr node = expression();

	while (!is_type<ErrorNode>(node) && current_token->type == TokenType::EQUAL)
	{
		if (has_equal)
			return error("statement has more than one equal sign");
		else
			has_equal = true;
		
		TokenType token_type = current_token->type;
		get_next_token();

		INodePtr right = expression();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}


// PLUS, MINUS
INodePtr Parser::expression()
{
	INodePtr node = term();

	while (!is_type<ErrorNode>(node)
			&& (current_token->type == TokenType::PLUS
			|| is_minus()))
	{
		TokenType token_type = current_token->type;
		get_next_token();
		INodePtr right = term();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}

// MULTIPLY, DIVIDE, MODULO
INodePtr Parser::term()
{
	INodePtr node = power();

	while (!is_type<ErrorNode>(node)
			&& (current_token->type == TokenType::MULTIPLY
			|| current_token->type == TokenType::DIVIDE
			|| current_token->type == TokenType::MODULO))
	{
		TokenType token_type = current_token->type;
		get_next_token();
		INodePtr right = power();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}

// POWER
INodePtr Parser::power()
{
	INodePtr node = factor();

	while (!is_type<ErrorNode>(node)
			&& current_token->type == TokenType::POWER)
	{
		TokenType token_type = current_token->type;
		get_next_token();
		INodePtr right = factor();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}

// NUMBER, VARIABLE
INodePtr Parser::factor()
{
	bool neg = false;
	if (current_token->type == TokenType::MINUS)
	{
		get_next_token();
		if (current_token->type != TokenType::NUMBER
				&& current_token->type != TokenType::VARIABLE
				&& current_token->type != TokenType::LPAREN)
			return error("invalid minus sign");
		neg = true;
	}

	INodePtr node;

	if (current_token->type == TokenType::NUMBER
			|| current_token->type == TokenType::LPAREN)
		node = natural_form();
	else if (current_token->type == TokenType::VARIABLE)
		node = variable();
	else
		return error("invalid type");

	if (neg)
	{
		auto neg_node = std::make_shared<ExprNode>(-1);
		return std::make_shared<OptNode>(TokenType::MULTIPLY, neg_node, node);
	}
	return node;
}

INodePtr Parser::parenthesis()
{
	get_next_token();
	auto node = expression();

	if (current_token->type != TokenType::RPAREN)
		return error("parenthesis not matching");
	get_next_token();

	return node;
}

INodePtr Parser::number()
{
	auto node = std::make_shared<ExprNode>(current_token->num_value);
	get_next_token();
	return node;
}

INodePtr Parser::variable()
{
	auto name = current_token->str_value;
	if (var_name == "")
		var_name = name;
	else if (var_name != name)
		return error("too much variable");
	auto node = std::make_shared<ExprNode>(current_token->str_value);
	get_next_token();
	return node;
}

INodePtr Parser::natural_form()
{
	INodePtr coef;
	if (current_token->type == TokenType::NUMBER)
		coef = number();
	else if (current_token->type == TokenType::LPAREN)
		coef = parenthesis();

	if (current_token->type != TokenType::VARIABLE)
		return coef;
	auto var = variable();

	if (current_token->type == TokenType::POWER)
	{
		get_next_token();
		auto degree = factor();
		auto right = std::make_shared<OptNode>(TokenType::POWER, var, degree);
		return std::make_shared<OptNode>(TokenType::MULTIPLY, coef, right);
	}

	return std::make_shared<OptNode>(TokenType::MULTIPLY, coef, var);
}