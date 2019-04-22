#pragma once
#include <boost/variant.hpp>
#include <iostream>
#include <string>

enum	TokenType
{
	NUMBER,
	VARIABLE,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	MODULO,
	POWER,
	LPAREN,
	RPAREN,
	EQUAL,
	ERROR,
	END
};

std::ostream & operator<<(std::ostream & o, TokenType const & t);

struct Token
{
	enum TokenType type;
	boost::variant
	<
		char,
		double,
		std::string
	>	value;
};

std::ostream & operator<<(std::ostream & o, Token const & t);