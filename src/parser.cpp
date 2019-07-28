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
	if (is_type<ErrorNode>(ast))
		return ast;
	if (current_token->type != TokenType::END)
		// TODO make error message more clear
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
std::shared_ptr<INode> Parser::error(std::string error_message)
{
	return std::dynamic_pointer_cast<INode>(std::make_shared<ErrorNode>(error_message));
}

// EQUAL
std::shared_ptr<INode> Parser::equation()
{
	std::shared_ptr<INode> node = expression();

	// TODO after expression can other thing left out to polute here
	while (!is_type<ErrorNode>(node) && current_token->type == TokenType::EQUAL)
	{
		if (has_equal)
			return error("statement has more than one equal sign");
		else
			has_equal = true;
		
		TokenType token_type = current_token->type;
		get_next_token();

		std::shared_ptr<INode> right = expression();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}


// PLUS, MINUS
std::shared_ptr<INode> Parser::expression()
{
	std::shared_ptr<INode> node = term();

	// TODO handle negative number
	while (!is_type<ErrorNode>(node)
			&& (current_token->type == TokenType::PLUS
			|| is_minus()))
	{
		TokenType token_type = current_token->type;
		get_next_token();
		std::shared_ptr<INode> right = term();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		else
			node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}

// MULTIPLY, DIVIDE, MODULO
std::shared_ptr<INode> Parser::term()
{
	std::shared_ptr<INode> node = power();

	while (!is_type<ErrorNode>(node)
			&& (current_token->type == TokenType::MULTIPLY
			|| current_token->type == TokenType::DIVIDE
			|| current_token->type == TokenType::MODULO))
	{
		TokenType token_type = current_token->type;
		get_next_token();
		std::shared_ptr<INode> right = power();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		// TODO handle modulo combination
		node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}

// POWER
std::shared_ptr<INode> Parser::power()
{
	std::shared_ptr<INode> node = factor();

	while (!is_type<ErrorNode>(node)
			&& current_token->type == TokenType::POWER)
	{
		TokenType token_type = current_token->type;
		get_next_token();
		std::shared_ptr<INode> right = factor();

		auto err = std::dynamic_pointer_cast<ErrorNode>(right);
		if (err != nullptr)
			return error(err->message);
		node = std::make_shared<OptNode>(token_type, node, right);
	}
	return node;
}

// NUMBER, VARIABLE
std::shared_ptr<INode> Parser::factor()
{
	auto v = Value::POSITIVE;
	if (current_token->type == TokenType::MINUS)
	{
		get_next_token();
		if (current_token->type != TokenType::NUMBER && current_token->type != TokenType::VARIABLE)
			return error("invalid minus sign");
		v = Value::NEGATIVE;
	}

	if (current_token->type == TokenType::NUMBER)
		return get_natural_form(v);

	if (current_token->type == TokenType::VARIABLE)
		return get_variable_node(v);

	if (current_token->type == TokenType::LPAREN)
	{
		get_next_token();
		auto node = expression();

		if (current_token->type != TokenType::RPAREN)
			return error("parenthesis not matching");
		get_next_token();

		return node;
	}

	return error("invalid type");
}

std::shared_ptr<INode> Parser::get_number_node(enum Value v)
{
	ExprPtr node;
	if (v == NEGATIVE)
		node = std::make_shared<ExprNode>(-current_token->num_value);
	else
		node = std::make_shared<ExprNode>(current_token->num_value);
	get_next_token();
	return node;
}

std::shared_ptr<INode> Parser::get_variable_node(enum Value v)
{
	auto name = current_token->str_value;
	if (var_name == "")
		var_name = name;
	else if (var_name != name)
		return error("too much variable");
	ExprPtr node;
	if (v == NEGATIVE)
		node = std::make_shared<ExprNode>(-1, current_token->str_value);
	else
		node = std::make_shared<ExprNode>(current_token->str_value);
	get_next_token();
	return node;
}

INodePtr Parser::get_natural_form(enum Value v)
{
	// this will alway be number so ok
	auto coef = get_number_node(v);
	// the problem is it doesn't handle left parenthesis

	if (current_token->type != TokenType::VARIABLE)
		return coef;
	auto var = get_variable_node(Value::POSITIVE);

	if (current_token->type == TokenType::POWER)
	{
		get_next_token();
		if (current_token->type != TokenType::NUMBER)
			return error("");

		auto degree = get_number_node(Value::POSITIVE);
		auto right = std::make_shared<OptNode>(TokenType::POWER, var, degree);
		return std::make_shared<OptNode>(TokenType::MULTIPLY, coef, right);
	}

	return std::make_shared<OptNode>(TokenType::MULTIPLY, coef, var);
}