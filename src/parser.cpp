#include "parser.hpp"

Parser::Parser(Lexer & lexer) :
	lexer(lexer),
	current_token(this->lexer.get_next_token())
{}

INode * Parser::parse()
{
	return priority1();
}

// prority_1: POWER
INode * Parser::priority1()
{
	INode * node = priority2();
	current_token = lexer.get_next_token();
	OperationNode * power_node = new OperationNode(TokenType::POWER, node, priority2());
	return power_node;
}

// priority2: NUMBER
INode * Parser::priority2()
{
	TermNode * node = new TermNode(boost::get<double>(current_token->value));
	current_token = lexer.get_next_token();
	return node;
}


/*
=
^
* /
+ -
number / variable

future
modulo
parenthesis
*/