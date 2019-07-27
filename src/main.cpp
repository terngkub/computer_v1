#include "main.hpp"
#include <string>
#include "interpreter.hpp"

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
        
    auto interpreter = Interpreter(ast);
    std::cout << "input        : " << input << "\n";
    interpreter.interpret();
}