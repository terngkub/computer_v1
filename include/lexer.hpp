#pragma once
#include <iterator>
#include <memory>
#include <string>
#include "token.hpp"

struct Lexer
{
    Lexer(std::string);
    std::shared_ptr<Token> get_next_token();

private:
    std::string str;
    std::string::iterator it;

	std::shared_ptr<Token> get_number();
    std::shared_ptr<Token> get_variable();
    std::shared_ptr<Token> get_error();
};