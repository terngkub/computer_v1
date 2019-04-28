#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"


struct Parser
{
	Parser(Lexer &);
	INode * parse();

private:
	typedef INode * (Parser::*ParserFunc)();

	Lexer &		lexer;
	Token *		current_token;
	std::string	var_name;
	bool		has_equal;

	INode *		equation();
	INode *		expression();
	INode *		term();
	INode *		power();
	INode *		factor();

};