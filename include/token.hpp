#pragma once
#include <iostream>
#include <memory>
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
	Token(enum TokenType type);
	Token(enum TokenType type, double num_value);
	Token(enum TokenType type, std::string str_value);

	enum TokenType	type;
	double			num_value;
	std::string		str_value;

	std::string str() const;
};

std::ostream & operator<<(std::ostream & o, Token const & t);

typedef std::shared_ptr<Token> TokenPtr;