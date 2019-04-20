#include "main.hpp"

void test_token()
{
    Token token_num = {Token::NUMBER, 10.0};
    std::cout << token_num << '\n';

    Token token_opt = {Token::OPERATOR, '+'};
    std::cout << token_opt << '\n';

    Token token_var = {Token::VARIABLE, "Var"};
    std::cout << token_var << '\n';
}

void test_lexer()
{
    std::string input;
    std::cin >> input;

    Lexer lexer(input);
    Token token = lexer.get_next_token();

    while (token.type != Token::ERROR && token.type != Token::END)
    {
        std::cout << token << '\n';
        token = lexer.get_next_token();
    }
}

int main()
{
    test_lexer();
}