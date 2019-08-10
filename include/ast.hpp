#pragma once
#include "token.hpp"
#include <map>
#include <memory>
#include <string>

// INode

struct INode
{
	virtual ~INode() {};
};
typedef std::shared_ptr<INode> INodePtr;

template <typename T>
bool is_type(INodePtr node)
{
	return (std::dynamic_pointer_cast<T>(node) != nullptr) ? true : false;
}

std::ostream & operator<<(std::ostream & o, INodePtr node);



// ErrorNode

struct ErrorNode : public INode
{
	std::string message;

	ErrorNode(std::string message);
};
typedef std::shared_ptr<ErrorNode> ErrorPtr;
std::ostream & operator<<(std::ostream & o, ErrorNode const & t);



// OptNode

struct OptNode : public INode
{
	enum TokenType	op;
	INodePtr		left;
	INodePtr		right;

	OptNode(enum TokenType, INodePtr, INodePtr);
};
typedef std::shared_ptr<OptNode> OptPtr;
std::ostream & operator<<(std::ostream & o, OptNode const & t);



// ExprNode

struct ExprNode : public INode
{
	std::map<int, double> term_map;
	std::string var_name;

	ExprNode(double nb);
	ExprNode(std::string var_name);

	bool contain_variable() const;
	void clean_map();
	bool remove_negative_degree();
	bool remove_excess_degree();
};
typedef std::shared_ptr<ExprNode> ExprPtr;

ExprNode operator+(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator-(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator*(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator/(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator^(ExprNode const & lhs, ExprNode const & rhs);
bool operator==(ExprNode const & lhs, ExprNode const & rhs);
std::ostream & operator<<(std::ostream & o, ExprNode const & t);
