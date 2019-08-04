#include "interpreter.hpp"
#include "main.hpp"
#include "output.hpp"
#include <string>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: ./computor [string]\n";
        return 1;
    }
    std::string input(argv[1]);

    Lexer lexer(input);
    Parser parser(lexer);
    auto ast = parser.parse();
    auto err = std::dynamic_pointer_cast<ErrorNode>(ast);
    if (err)
    {
        std::cerr << err->message << '\n';
        return 1;
    }
    std::cout << "parsed form  : " << ast << "\n";

    Interpreter interpreter(ast);
    auto output = interpreter.interpret();
    std::cout << output;
}