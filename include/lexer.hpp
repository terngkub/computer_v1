#pragma once
#include <iterator>
#include <string>
#include "token.hpp"

class Lexer
{
    std::string str;
    std::string::iterator it;

	Token get_number();
    Token get_variable();

public:
    Lexer(std::string);
    Token get_next_token();
};