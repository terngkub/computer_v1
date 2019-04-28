#pragma once
#include "token.hpp"
#include <string>

struct INode
{
public:
	~INode() {};
	virtual void print() {};
};

struct TermNode : public INode
{
	std::string	name;
	double		coef;
	int			power;

	TermNode(std::string name);
	TermNode(double coef);
	TermNode(double coef, int power);

	void print();
};

std::ostream & operator<<(std::ostream & o, TermNode const & t);

struct OperationNode : public INode
{
	enum TokenType	op;
	INode *			left;
	INode *			right;

	OperationNode(enum TokenType, INode *, INode *);

	void print();
};

struct ErrorNode : public INode
{
	std::string message;

	ErrorNode(std::string message);
};