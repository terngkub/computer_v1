#pragma once
#include <boost/variant.hpp>
#include <iostream>
#include <string>


struct Token
{
	enum
	{
		NUMBER,
		OPERATOR,
		VARIABLE,
		EQUAL,
		LPAREN,
		RPAREN,
		ERROR,
		END
	}	type;

	boost::variant
	<
		char,
		double,
		std::string
	>	value;
};

std::ostream & operator<<(std::ostream & o, Token const & t);