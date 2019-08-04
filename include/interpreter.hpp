#pragma once
#include "ast.hpp"
#include "complex.hpp"
#include "output.hpp"
#include <list>
#include <memory>
#include <string>

class Interpreter
{
	INodePtr ast;
	ExprPtr result;
	bool has_divide;
	bool has_equal;
	bool has_excess_degree;
	bool has_negative_degree;
	std::list<Complex> solution;

	INodePtr visit(INodePtr node);
	INodePtr operate(OptPtr op_node, ExprPtr left, ExprPtr right);

	void clean_result();

	void solve();
	void solve_one_degree();
	void solve_two_degree();
	void get_two_degree_solution();

	Output		get_output();
	std::string	get_reduced_form();
	int			get_degree();
	std::string	get_solution();

public:
	Interpreter(std::shared_ptr<INode>);
	Output interpret();
};