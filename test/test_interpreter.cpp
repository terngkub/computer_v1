#include "catch.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

Output get_output(std::string input)
{
    Lexer lexer(input);
    Parser parser(lexer);
    Interpreter interpreter(parser.parse());
    return interpreter.interpret();
}

TEST_CASE("zero degree", "[interpreter]")
{
	SECTION("only number")
    {
        auto output = get_output("42");
        REQUIRE(output.reduced_form == "42");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "42");
        REQUIRE(output.error == "");
    }
    SECTION("calculation")
    {
        auto output = get_output("(3*21--1)/(-3+1)^3");
        REQUIRE(output.reduced_form == "-8");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "-8");
        REQUIRE(output.error == "");
    }
    SECTION("zero")
    {
        auto output = get_output("42 - 42");
        REQUIRE(output.reduced_form == "0");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "0");
        REQUIRE(output.error == "");
    }
}

TEST_CASE("one degree", "[interpreter]")
{
    SECTION("simple")
    {
        auto output = get_output("x + 2");
        REQUIRE(output.reduced_form == "x + 2 = 0");
        REQUIRE(output.degree == 1);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = -2");
        REQUIRE(output.error == "");
    }
    SECTION("coef")
    {
        auto output = get_output("4x - 1");
        REQUIRE(output.reduced_form == "4x - 1 = 0");
        REQUIRE(output.degree == 1);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = 0.25");
        REQUIRE(output.error == "");
    }
}

TEST_CASE("two degree", "[interpreter]")
{
    SECTION("one solution")
    {
        auto output = get_output("x^2 + 2x + 1");
        REQUIRE(output.reduced_form == "x^2 + 2x + 1 = 0");
        REQUIRE(output.degree == 2);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = -1");
        REQUIRE(output.error == "");
    }
    SECTION("two solution")
    {
        auto output = get_output("x^2 + 3x + 2");
        REQUIRE(output.reduced_form == "x^2 + 3x + 2 = 0");
        REQUIRE(output.degree == 2);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = -1, -2");
        REQUIRE(output.error == "");
    }
    SECTION("two solution with minus and coef")
    {
        auto output = get_output("2x^2 + x - 1");
        REQUIRE(output.reduced_form == "2x^2 + x - 1 = 0");
        REQUIRE(output.degree == 2);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = 0.5, -1");
        REQUIRE(output.error == "");
    }
    SECTION("imaginary solution")
    {
        auto output = get_output("x^2 + 2x + 2");
        REQUIRE(output.reduced_form == "x^2 + 2x + 2 = 0");
        REQUIRE(output.degree == 2);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = -1 + i, -1 - i");
        REQUIRE(output.error == "");
    }
}

TEST_CASE("special case", "[interpreter]")
{
    SECTION("too much degree")
    {
        auto output = get_output("x^3 + 3x^2 + 3x + 1");
        REQUIRE(output.reduced_form == "x^3 + 3x^2 + 3x + 1 = 0");
        REQUIRE(output.degree == 3);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "cannot handle degree greater than two");
        REQUIRE(output.error == "");
    }
    SECTION("excess degree")
    {
        auto output = get_output("x^3 + 3x^2 + 2x");
        REQUIRE(output.reduced_form == "(x)(x^2 + 3x + 2) = 0");
        REQUIRE(output.degree == 3);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x = -1, -2, 0");
        REQUIRE(output.error == "");
    }
    SECTION("negative degree")
    {
        auto output = get_output("1/x");
        REQUIRE(output.reduced_form == "x = 0");
        REQUIRE(output.degree == 1);
        REQUIRE(output.has_limit == true);
        REQUIRE(output.solution == "no solution");
        REQUIRE(output.error == "");
    }
    SECTION("excess degree with division")
    {
        auto output = get_output("(x^3 + x^2) / x");
        REQUIRE(output.reduced_form == "(x)(x + 1) = 0");
        REQUIRE(output.degree == 2);
        REQUIRE(output.has_limit == true);
        REQUIRE(output.solution == "x = -1");
        REQUIRE(output.error == "");
    }
    SECTION("equation with no degree")
    {
        auto output = get_output("4 - 3 = 0");
        REQUIRE(output.reduced_form == "");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "");
        REQUIRE(output.error == "need variable in equation");
    }
    SECTION("solution can be any numbers, no equation")
    {
        auto output = get_output("x - x");
        REQUIRE(output.reduced_form == "0 = 0");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x can be any number");
        REQUIRE(output.error == "");
    }
    SECTION("solution can be any numbers, equation")
    {
        auto output = get_output("x = x");
        REQUIRE(output.reduced_form == "0 = 0");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == false);
        REQUIRE(output.solution == "x can be any number");
        REQUIRE(output.error == "");
    }
    SECTION("solution can be any numbers, division")
    {
        auto output = get_output("1/x = 1/x");
        REQUIRE(output.reduced_form == "0 = 0");
        REQUIRE(output.degree == 0);
        REQUIRE(output.has_limit == true);
        REQUIRE(output.solution == "x can be any number");
        REQUIRE(output.error == "");
    }
}