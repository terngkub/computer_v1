#include "lexer.hpp"

Token Lexer::get_number()
{
	std::string buf;
	bool dot = false;
	while (it != str.end() && (isdigit(*it) || *it == '.'))
	{
		if (*it == '.')
		{
			if (dot)
				return Token{Token::ERROR, "Invalid character"};
			dot = true;
		}
		buf.append(1, *it);
		++it;
	}
	double value = stod(buf);
	return Token{Token::NUMBER, value};
}

Token Lexer::get_variable()
{
	std::string buf;
	while (it != str.end() && isalpha(*it))
	{
		buf.append(1, *it);
		++it;
	}
	return Token{Token::VARIABLE, buf};
}

Lexer::Lexer(std::string text) :
	str(text),
	it(str.begin())
{}

Token Lexer::get_next_token()
{
	while (it != str.end() && isspace(*it))
		++it;
	if (it == str.end())
		return Token{Token::END, "end"};
	
	if (isdigit(*it) || (*it == '-' && it + 1 != str.end() && isdigit(*(it + 1))))
		return get_number();
	if (isalpha(*it))
		return get_variable();
	switch(*it)
	{
		case '+':
			return Token{Token::OPERATOR, *it++};
		case '-':
			return Token{Token::OPERATOR, *it++};
		case '*':
			return Token{Token::OPERATOR, *it++};
		case '/':
			return Token{Token::OPERATOR, *it++};
		case '^':
			return Token{Token::OPERATOR, *it++};
		case '=':
			return Token{Token::EQUAL, *it++};
		case '(':
			return Token{Token::LPAREN, *it++};
		case ')':
			return Token{Token::RPAREN, *it++};
	}

	return Token{Token::ERROR, "Invalid character"};
}
