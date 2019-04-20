#include "lexer.hpp"

Token * Lexer::get_number()
{
	std::string buf;
	bool dot = false;
	while (it != str.end() && (isdigit(*it) || *it == '.'))
	{
		if (*it == '.')
		{
			if (dot)
				return new Token{TokenType::ERROR, "Invalid character"};
			dot = true;
		}
		buf.append(1, *it);
		++it;
	}
	double value = stod(buf);
	return new Token{TokenType::NUMBER, value};
}

Token * Lexer::get_variable()
{
	std::string buf;
	while (it != str.end() && isalpha(*it))
	{
		buf.append(1, *it);
		++it;
	}
	return new Token{TokenType::VARIABLE, buf};
}

Lexer::Lexer(std::string text) :
	str(text),
	it(str.begin())
{}

Token * Lexer::get_next_token()
{
	while (it != str.end() && isspace(*it))
		++it;
	if (it == str.end())
		return new Token{TokenType::END, "end"};
	
	if (isdigit(*it) || (*it == '-' && it + 1 != str.end() && isdigit(*(it + 1))))
		return get_number();
	if (isalpha(*it))
		return get_variable();
	switch(*it)
	{
		case '+':
			return new Token{TokenType::PLUS, *it++};
		case '-':
			return new Token{TokenType::MINUS, *it++};
		case '*':
			return new Token{TokenType::MULTIPLY, *it++};
		case '/':
			return new Token{TokenType::DIVIDE, *it++};
		case '^':
			return new Token{TokenType::POWER, *it++};
		case '=':
			return new Token{TokenType::EQUAL, *it++};
		case '(':
			return new Token{TokenType::LPAREN, *it++};
		case ')':
			return new Token{TokenType::RPAREN, *it++};
	}

	return new Token{TokenType::ERROR, "Invalid character"};
}
