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
	std::cout << "token: " << current_token->str() << "\n";
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