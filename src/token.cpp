#include "token.hpp"

std::ostream & operator<<(std::ostream & o, Token const & token)
{
	o << token.value;
	return o;
}
