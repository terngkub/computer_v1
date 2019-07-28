#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <memory>

enum Value
{
	POSITIVE,
	NEGATIVE
};

struct Parser
{
	Parser(Lexer &);
	std::shared_ptr<INode>	parse();

private:
	typedef std::shared_ptr<INode> (Parser::*ParserFunc)();

	Lexer &					lexer;
	std::shared_ptr<Token>	prev_token;
	std::shared_ptr<Token>	current_token;
	std::string				var_name;
	bool					has_equal;

	void 					get_next_token();
	bool 					is_minus();
	std::shared_ptr<INode>	error(std::string error_message);

	std::shared_ptr<INode>	equation();
	std::shared_ptr<INode>	expression();
	std::shared_ptr<INode>	term();
	std::shared_ptr<INode>	power();
	std::shared_ptr<INode>	factor();
	INodePtr parenthesis();

	std::shared_ptr<INode> get_number_node(enum Value v);
	std::shared_ptr<INode> get_variable_node(enum Value v);
	INodePtr get_natural_form(enum Value v);
};