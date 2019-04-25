#pragma once
#include "token.hpp"

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

struct OperationNode : public INode
{
	enum TokenType	op;
	INode *			left;
	INode *			right;

	OperationNode(enum TokenType, INode *, INode *);

	void print();
};