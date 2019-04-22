#pragma once
#include "token.hpp"

class INode
{
public:
	virtual void print() {};
};

class TermNode : public INode
{
	std::string	name;
	double		coef;
	int			power;

public:
	TermNode(std::string name);
	TermNode(double coef);
	void print();
};

struct OperationNode : public INode
{
	enum TokenType	op;
	INode *			left;
	INode *			right;

public:
	OperationNode(enum TokenType, INode *, INode *);
	void print();
};