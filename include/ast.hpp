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

template <typename T>
bool is_type(std::shared_ptr<INode> node)
{
	return (std::dynamic_pointer_cast<T>(node) != nullptr) ? true : false;
}



// ErrorNode

struct ErrorNode : public INode
{
	std::string message;

	ErrorNode(std::string message);
};

std::ostream & operator<<(std::ostream & o, ErrorNode const & t);



// OptNode

struct OptNode : public INode
{
	enum TokenType			op;
	std::shared_ptr<INode>	left;
	std::shared_ptr<INode>	right;

	OptNode(enum TokenType, std::shared_ptr<INode>, std::shared_ptr<INode>);
};

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

ExprNode operator+(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator-(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator*(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator/(ExprNode const & lhs, ExprNode const & rhs);
ExprNode operator^(ExprNode const & lhs, ExprNode const & rhs);
bool operator==(ExprNode const & lhs, ExprNode const & rhs);
std::ostream & operator<<(std::ostream & o, ExprNode const & t);



// Typedef

typedef std::shared_ptr<INode> INodePtr;
typedef std::shared_ptr<ErrorNode> ErrorPtr;
typedef std::shared_ptr<OptNode> OptPtr;
typedef std::shared_ptr<ExprNode> ExprPtr;