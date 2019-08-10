#pragma once
#include <iterator>
#include <memory>
#include <string>
#include "token.hpp"

struct Lexer
{
    Lexer(std::string);
    TokenPtr get_next_token();

private:
    std::string str;
    std::string::iterator it;

	TokenPtr get_number();
    TokenPtr get_variable();
    TokenPtr get_error();
};