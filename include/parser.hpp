#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

class Parser
{
	Lexer &	lexer;
	Token *	current_token;
	bool has_equal;

	INode * equation();
	INode * expression();
	INode * term();
	INode * power();
	INode * factor();


public:
	Parser(Lexer &);
	INode * parse();
};