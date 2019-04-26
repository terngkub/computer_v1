#pragma once
#include <iterator>
#include <string>
#include "token.hpp"

struct Lexer
{
    Lexer(std::string);
    Token * get_next_token();

private:
    std::string str;
    std::string::iterator it;

	Token * get_number();
    Token * get_variable();
    Token * get_error();
};