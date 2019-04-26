#include "token.hpp"

std::ostream & operator<<(std::ostream & o, TokenType const & t)
{
	switch(t)
	{
		case TokenType::NUMBER:		o << "NUMBER"; break;
		case TokenType::VARIABLE:	o << "VARIABLE"; break;
		case TokenType::PLUS:		o << "PLUS"; break;
		case TokenType::MINUS:		o << "MINUS"; break;
		case TokenType::MULTIPLY:	o << "MULTIPLY"; break;
		case TokenType::DIVIDE:		o << "DIVIDE"; break;
		case TokenType::MODULO:		o << "MODULO"; break;
		case TokenType::POWER:		o << "POWER"; break;
		case TokenType::LPAREN:		o << "LPAREN"; break;
		case TokenType::RPAREN:		o << "RPAREN"; break;
		case TokenType::EQUAL:		o << "EQUAL"; break;
		case TokenType::ERROR:		o << "ERROR"; break;
		case TokenType::END:		o << "END"; break;
		default: break;
	}
	return o;
}

Token::Token(TokenType type) :
	type(type),
	num_value(0)
{
	switch(type)
	{
		case TokenType::PLUS:		str_value = "+"; break;
		case TokenType::MINUS:		str_value = "-"; break;
		case TokenType::MULTIPLY:	str_value = "*"; break;
		case TokenType::DIVIDE:		str_value = "/"; break;
		case TokenType::MODULO:		str_value = "%"; break;
		case TokenType::POWER:		str_value = "^"; break;
		case TokenType::LPAREN:		str_value = "("; break;
		case TokenType::RPAREN:		str_value = ")"; break;
		case TokenType::EQUAL:		str_value = "="; break;
		case TokenType::END:		str_value = "end"; break;
		default: break;
	}
}

Token::Token(TokenType type, double num_value) :
	type(type),
	num_value(num_value),
	str_value("")
{}

Token::Token(TokenType type, std::string str_value) :
	type(type),
	num_value(0),
	str_value(str_value)
{}

std::ostream & operator<<(std::ostream & o, Token const & token)
{
	o << token.type;
	if (token.type == TokenType::NUMBER)
		o << token.num_value;
	else
		o << token.str_value;
	return o;
}