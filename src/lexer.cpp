#include "lexer.hpp"
#include <sstream>

Lexer::Lexer(std::string text) :
	str(text),
	it(str.begin())
{}

std::shared_ptr<Token> Lexer::get_next_token()
{
	while (it != str.end() && isspace(*it))
		++it;
	if (it == str.end())
		return std::make_shared<Token>(TokenType::END);
	if (isdigit(*it))
		return get_number();
	if (isalpha(*it))
		return get_variable();
	switch(*it)
	{
		case '+': ++it; return std::make_shared<Token>(TokenType::PLUS);
		case '-': ++it; return std::make_shared<Token>(TokenType::MINUS);
		case '*': ++it; return std::make_shared<Token>(TokenType::MULTIPLY);
		case '/': ++it; return std::make_shared<Token>(TokenType::DIVIDE);
		case '%': ++it; return std::make_shared<Token>(TokenType::MODULO);
		case '^': ++it; return std::make_shared<Token>(TokenType::POWER);
		case '(': ++it; return std::make_shared<Token>(TokenType::LPAREN);
		case ')': ++it; return std::make_shared<Token>(TokenType::RPAREN);
		case '=': ++it; return std::make_shared<Token>(TokenType::EQUAL);
	}
	return get_error();
}

std::shared_ptr<Token> Lexer::get_number()
{
	std::string buf;
	bool dot = false;
	while (it != str.end() && (isdigit(*it) || *it == '.'))
	{
		if (*it == '.')
		{
			if (dot)
				return get_error();
			dot = true;
		}
		buf.append(1, *it);
		++it;
	}
	double value = stod(buf);
	return std::make_shared<Token>(TokenType::NUMBER, value);
}

std::shared_ptr<Token> Lexer::get_variable()
{
	std::string buf;
	while (it != str.end() && isalpha(*it))
	{
		buf.append(1, *it);
		++it;
	}
	return std::make_shared<Token>(TokenType::VARIABLE, buf);
}

std::shared_ptr<Token> Lexer::get_error()
{
	std::stringstream err;
	err << "invalid character '" << *it << "' at index " << it - str.begin();
	return std::make_shared<Token>(TokenType::ERROR, err.str());
}
