#include "ast.hpp"

// INode
std::ostream & operator<<(std::ostream & o, std::shared_ptr<INode> node)
{
    auto err_node = std::dynamic_pointer_cast<ErrorNode>(node);
    if (err_node)
	{
        o << *err_node;
		return o;
	}
    auto opt_node = std::dynamic_pointer_cast<OptNode>(node);
    if (opt_node)
	{
        o << *opt_node;
		return o;
	}
    auto expr_node = std::dynamic_pointer_cast<ExprNode>(node);
    if (expr_node)
	{
        o << *expr_node;
		return o;
	}
	return o;
}


// ErrorNode

ErrorNode::ErrorNode(std::string message) :
	message(message)
{}

std::ostream & operator<<(std::ostream & o, ErrorNode const & node)
{
	o << node.message;
	return o;
}



// OptNode

OptNode::OptNode(enum TokenType op, std::shared_ptr<INode> left, std::shared_ptr<INode> right) :
	op(op),
	left(left),
	right(right)
{}

std::ostream & operator<<(std::ostream & o, OptNode const & node)
{
	o << "(" << node.left;
	switch(node.op)
	{
		case TokenType::PLUS:		o << " + "; break;
		case TokenType::MINUS:		o << " - "; break;
		case TokenType::MULTIPLY:	o << " * "; break;
		case TokenType::DIVIDE:		o << " / "; break;
		case TokenType::MODULO:		o << " % "; break;
		case TokenType::POWER:		o << "^"; break;
		case TokenType::EQUAL:		o << " = "; break;
		default: break;
	}
	o << node.right << ")";
	return o;
}



// ExprNode

ExprNode::ExprNode(double nb) :
	var_name("")
{
	term_map[0] = nb;
}

ExprNode::ExprNode(std::string var_name) :
	var_name(var_name)
{
	term_map[1] = 1;
}

bool ExprNode::contain_variable() const
{
    if (term_map.size() > 1)
		return true;
    if (term_map.size() == 1 && term_map.find(0) == term_map.end())
	{
        return true;
	}
	return false;
}

void ExprNode::clean_map()
{
	for (auto elem : term_map)
	{
		if (elem.second == 0)
			term_map.erase(elem.first);
	}
	if (term_map.size() == 0)
		term_map[0] = 0;
}

bool ExprNode::remove_negative_degree()
{
	int shift_nb = term_map.begin()->first;
	if (shift_nb >= 0)
		return false;
	if (term_map.size() == 1)
		shift_nb -= 1;

	auto tmp_map = term_map;
	term_map.clear();
	for (auto elem : tmp_map)
		term_map[elem.first - shift_nb] = elem.second;
	clean_map();
	return true;
}

bool ExprNode::remove_excess_degree()
{
	int shift_nb = term_map.begin()->first;
	if (shift_nb < 1)
		return false;
	if (term_map.size() == 1)
		shift_nb -= 1;

	auto tmp_map = term_map;
	term_map.clear();
	for (auto elem: tmp_map)
		term_map[elem.first - shift_nb] = elem.second;
	clean_map();
	return true;
}

ExprNode operator+(ExprNode const & lhs, ExprNode const & rhs)
{
	ExprNode ret(lhs);

	if (rhs.var_name != "")
		ret.var_name = rhs.var_name;

	for (auto elem : rhs.term_map)
		ret.term_map[elem.first] += elem.second; 

	ret.clean_map();
	return ret;
}

ExprNode operator-(ExprNode const & lhs, ExprNode const & rhs)
{
	ExprNode ret(lhs);

	if (rhs.var_name != "")
		ret.var_name = rhs.var_name;

	for (auto elem : rhs.term_map)
		ret.term_map[elem.first] -= elem.second; 

	ret.clean_map();
	return ret;
}

ExprNode operator*(ExprNode const & lhs, ExprNode const & rhs)
{
	ExprNode ret(0);
	ret.var_name = lhs.var_name != "" ? lhs.var_name : rhs.var_name;

	for (auto elem_l : lhs.term_map)
	{
		for (auto elem_r : rhs.term_map)
		{
			auto exponent = elem_l.first + elem_r.first;
			auto coef = elem_l.second * elem_r.second;
			ret.term_map[exponent] += coef;
		}
	}
	ret.clean_map();
	return ret;
}

ExprNode operator/(ExprNode const & lhs, ExprNode const & rhs)
{
	// divide by expression
	if (rhs.term_map.size() != 1)
		throw std::runtime_error("cannot divide by an expression containing more than one term");

	// divide by zero
	if (rhs.term_map.find(0) != rhs.term_map.end() && rhs.term_map.at(0) == 0)
		throw std::runtime_error("divide by zero");

	// divide by one
	if (rhs.term_map.begin()->second == 0)
		return lhs;

	ExprNode ret(0);
	ret.var_name = lhs.var_name != "" ? lhs.var_name : rhs.var_name;

	for (auto elem : lhs.term_map)
	{
		auto coef = elem.second / rhs.term_map.begin()->second;
		auto exponent = elem.first - rhs.term_map.begin()->first;
		ret.term_map[exponent] = coef;
	}
	ret.clean_map();
	return ret;
}

ExprNode operator^(ExprNode const & lhs, ExprNode const & rhs)
{
	if (rhs.contain_variable())
		throw std::runtime_error("operation '^': exponent cannot container variable(s)");

	auto exponent = rhs.term_map.at(0);

	if (exponent < 0)
		throw std::runtime_error("operation '^': exponent cannot be negative number");
	if (exponent == 0)
		return ExprNode(1);
	if (exponent ==  1)
		return lhs;

	auto ret = ExprNode(lhs);
	auto tmp = ExprNode(lhs);

	while (exponent-- > 1)
		ret = ret * tmp;
	ret.clean_map();
	return ret;
}

bool operator==(ExprNode const & lhs, ExprNode const & rhs)
{
	if (lhs.term_map.size() != rhs.term_map.size())
		return false;
	if (lhs.var_name != rhs.var_name)
		return false;
	for (auto elem : lhs.term_map)
	{
		if (rhs.term_map.find(elem.first) == rhs.term_map.end())
			return false;
		if (rhs.term_map.at(elem.first) != elem.second)
			return false;
	}
	return true;
}

std::ostream & operator<<(std::ostream & o, ExprNode const & node)
{
	for (auto it = node.term_map.rbegin(); it != node.term_map.rend(); ++it)
	{
		bool neg = false;

		// plus/minus
		if (it != node.term_map.rbegin())
		{
			if (it->second < 0)
			{
				o << " - ";
				neg = true;
			}
			else
				o << " + ";
		}

		// coeff
		auto coef = neg ? -it->second : it->second;
		if (coef != 1 || it->first == 0)
			o << coef;

		// variable and power
		if (it->first != 0)
			o << node.var_name;
		if (it->first > 1)
			o << "^" << it->first;
		else if (it->first < 0)
			o << "^(" << it->first << ")";
		
	}
	return o;
}
