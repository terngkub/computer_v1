#include "token.hpp"

std::ostream & operator<<(std::ostream & o, TokenType const & t)
{
	switch(t)
	{
		case TokenType::PLUS: o << '+'; break;
		case TokenType::MINUS: o << '-'; break;
		case TokenType::MULTIPLY: o << '*'; break;
		case TokenType::DIVIDE: o << '/'; break;
		case TokenType::MODULO: o << '%'; break;
		case TokenType::POWER: o << '^'; break;
		case TokenType::LPAREN: o << '('; break;
		case TokenType::RPAREN: o << ')'; break;
		case TokenType::EQUAL: o << '='; break;
		default: break;
	}
	return o;
}

std::ostream & operator<<(std::ostream & o, Token const & token)
{
	o << token.value;
	return o;
}
