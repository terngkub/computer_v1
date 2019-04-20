#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

class Parser
{
	Lexer &	lexer;
	Token *	current_token;

	INode *	priority1();
	INode * priority2();

public:
	Parser(Lexer &);
	INode * parse();
};