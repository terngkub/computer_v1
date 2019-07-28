#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <memory>

struct Parser
{
	Parser(Lexer &);
	INodePtr	parse();

private:
	typedef std::shared_ptr<INode> (Parser::*ParserFunc)();

	Lexer &		lexer;
	TokenPtr	prev_token;
	TokenPtr	current_token;
	std::string	var_name;
	bool		has_equal;

	void 		get_next_token();
	bool 		is_minus();
	INodePtr	error(std::string error_message);

	INodePtr	equation();
	INodePtr	expression();
	INodePtr	term();
	INodePtr	power();
	INodePtr	factor();
	INodePtr	parenthesis();
	INodePtr	number();
	INodePtr	variable();
	INodePtr	natural_form();
};