#include "main.hpp"
#include <string>
#include "interpreter.hpp"

void test_token()
{
    Token token_num = {TokenType::NUMBER, 10.0};
    std::cout << token_num << '\n';

    Token token_opt = {TokenType::PLUS, '+'};
    std::cout << token_opt << '\n';

    Token token_var = {TokenType::VARIABLE, "Var"};
    std::cout << token_var << '\n';
}

void test_lexer()
{
    std::string input;
    getline(std::cin, input);

    Lexer lexer(input);
    auto token = lexer.get_next_token();

    while (token->type != TokenType::ERROR && token->type != TokenType::END)
    {
        std::cout << *token << '\n';
        token = lexer.get_next_token();
    }
}

void test_parser()
{
    std::string input;
    getline(std::cin, input);

    Lexer lexer(input);
    Parser parser(lexer);
    auto node = parser.parse();
    node->print();
}

void test_interpreter()
{
    std::string input;
    getline(std::cin, input);

    Lexer lexer(input);
    Parser parser(lexer);
    auto ast = parser.parse();
    auto err = std::dynamic_pointer_cast<ErrorNode>(ast);
    if (err)
    {
        std::cerr << err->message << '\n';
        return ;
    }
        
    auto interpreter = Interpreter(ast);
    interpreter.interpret();
}

int main()
{
    test_interpreter();
}