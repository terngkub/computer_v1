#include "catch.hpp"
#include "lexer.hpp"

TEST_CASE("lexer simple", "[lexer]")
{
    SECTION("integer")
    {
        auto lexer = Lexer("42");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 42.0);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);
    }
    SECTION("double")
    {
        auto lexer = Lexer("3.14");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 3.14);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);
    }
    SECTION("variable")
    {
        auto lexer = Lexer("abc");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "abc");
        REQUIRE(token->type == TokenType::VARIABLE);
    }
    SECTION("plus")
    {
        auto lexer = Lexer("+");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "+");
        REQUIRE(token->type == TokenType::PLUS);
    }
    SECTION("minus")
    {
        auto lexer = Lexer("-");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "-");
        REQUIRE(token->type == TokenType::MINUS);
    }
    SECTION("multiply")
    {
        auto lexer = Lexer("*");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "*");
        REQUIRE(token->type == TokenType::MULTIPLY);
    }
    SECTION("divide")
    {
        auto lexer = Lexer("/");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "/");
        REQUIRE(token->type == TokenType::DIVIDE
        );
    }
    SECTION("power")
    {
        auto lexer = Lexer("^");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "^");
        REQUIRE(token->type == TokenType::POWER);
    }
    SECTION("modulo")
    {
        auto lexer = Lexer("%");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "%");
        REQUIRE(token->type == TokenType::MODULO);
    }
    SECTION("end")
    {
        auto lexer = Lexer("");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "end");
        REQUIRE(token->type == TokenType::END);
    }
    SECTION("new line")
    {
        auto lexer = Lexer("\n");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "end");
        REQUIRE(token->type == TokenType::END);
    }
}

TEST_CASE("lexer error", "[lexer]")
{
    SECTION("two dot")
    {
        auto lexer = Lexer("1.2.3");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "invalid character '.' at index 3");
        REQUIRE(token->type == TokenType::ERROR);
    }
    SECTION("invalid char")
    {
        auto lexer = Lexer("~");
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "invalid character '~' at index 0");
        REQUIRE(token->type == TokenType::ERROR);
    }
}

TEST_CASE("lexer multiple", "[lexer]")
{
    SECTION("equation")
    {
        auto lexer = Lexer("3.14 %2++3- x^2.0= 3*4.0/5");

        // 3.14
        auto token = lexer.get_next_token();
        REQUIRE(token->num_value == 3.14);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);

        // %
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "%");
        REQUIRE(token->type == TokenType::MODULO);

        // 2
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 2);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);

        // +
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "+");
        REQUIRE(token->type == TokenType::PLUS);

        // +
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "+");
        REQUIRE(token->type == TokenType::PLUS);

        // 3
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 3);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);

        // -
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "-");
        REQUIRE(token->type == TokenType::MINUS);

        // x
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "x");
        REQUIRE(token->type == TokenType::VARIABLE);

        // ^
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "^");
        REQUIRE(token->type == TokenType::POWER);

        // 2.0
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 2.0);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);

        // =
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "=");
        REQUIRE(token->type == TokenType::EQUAL);

        // 3
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 3);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);

        // *
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "*");
        REQUIRE(token->type == TokenType::MULTIPLY);

        // 4.0
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 4.0);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);

        // /
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 0);
        REQUIRE(token->str_value == "/");
        REQUIRE(token->type == TokenType::DIVIDE);

        // 5
        token = lexer.get_next_token();
        REQUIRE(token->num_value == 5.0);
        REQUIRE(token->str_value == "");
        REQUIRE(token->type == TokenType::NUMBER);
    }
}