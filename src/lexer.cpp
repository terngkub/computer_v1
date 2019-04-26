#include "lexer.hpp"
#include <sstream>

Lexer::Lexer(std::string text) :
	str(text),
	it(str.begin())
{}

Token * Lexer::get_next_token()
{
	while (it != str.end() && isspace(*it))
		++it;
	if (it == str.end())
		return new Token(TokenType::END);
	if (isdigit(*it))
		return get_number();
	if (isalpha(*it))
		return get_variable();
	switch(*it)
	{
		case '+': ++it; return new Token(TokenType::PLUS);
		case '-': ++it; return new Token(TokenType::MINUS);
		case '*': ++it; return new Token(TokenType::MULTIPLY);
		case '/': ++it; return new Token(TokenType::DIVIDE);
		case '%': ++it; return new Token(TokenType::MODULO);
		case '^': ++it; return new Token(TokenType::POWER);
		case '(': ++it; return new Token(TokenType::LPAREN);
		case ')': ++it; return new Token(TokenType::RPAREN);
		case '=': ++it; return new Token(TokenType::EQUAL);
	}
	return get_error();
}

Token * Lexer::get_number()
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
	return new Token(TokenType::NUMBER, value);
}

Token * Lexer::get_variable()
{
	std::string buf;
	while (it != str.end() && isalpha(*it))
	{
		buf.append(1, *it);
		++it;
	}
	return new Token(TokenType::VARIABLE, buf);
}

Token * Lexer::get_error()
{
	std::stringstream err;
	err << "invalid character '" << *it << "' at index " << it - str.begin();
	return new Token(TokenType::ERROR, err.str());
}
