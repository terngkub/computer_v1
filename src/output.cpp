#include "output.hpp"
#include <iostream>

Output::Output()
{}

Output::Output(std::string error) :
  error(error)
{}

std::ostream & operator<<(std::ostream & o, Output const & output)
{
	if (!output.error.empty())
	{
		o << output.error << "\n";
		return o;
	}
	o << "reduced form : " << output.reduced_form << "\n";
	o << "degree       : " << output.degree << "\n";
    if (output.has_limit)
		o << "limitation   : x != 0\n";
    o << "solution:    : " << output.solution << "\n";
    return o;
}
