#include "catch.hpp"
#include "ast.hpp"

TEST_CASE("expression plus", "[expression]")
{
	SECTION("integer")
	{
		auto one = ExprNode(1);
		auto two = ExprNode(2);
		auto result = one + two;
		auto expected = ExprNode(3);
		REQUIRE(result == expected);
	}
	SECTION("expression")
	{
		// x + 2
		auto one = ExprNode("x");
		one.term_map[0] = 2;

		// 2x
		auto two = ExprNode("x");
		two.term_map[1] = 2;

		auto result = one + two;

		// 3x + 2
		auto expected = ExprNode("x");
		expected.term_map[1] = 3;
		expected.term_map[0] = 2;

		REQUIRE(result == expected);
	}
}

TEST_CASE("expression minus", "[expression]")
{
	SECTION("integer")
	{
		auto one = ExprNode(1);
		auto two = ExprNode(2);
		auto result = one - two;
		auto expected = ExprNode(-1);
		REQUIRE(result == expected);
	}
	SECTION("expression")
	{
		// x + 2
		auto one = ExprNode("x");
		one.term_map[0] = 2;

		// 2x
		auto two = ExprNode("x");
		two.term_map[1] = 2;

		auto result = one - two;

		// -x + 2
		auto expected = ExprNode("x");
		expected.term_map[1] = -1;
		expected.term_map[0] = 2;

		REQUIRE(result == expected);
	}
}

TEST_CASE("expression multiply", "[expression]")
{
	SECTION("integer")
	{
		auto one = ExprNode(1);
		auto two = ExprNode(2);
		auto result = one * two;
		auto expected = ExprNode(2);
		REQUIRE(result == expected);
	}
	SECTION("expression")
	{
		// 3x + 2
		auto one = ExprNode("x");
		one.term_map[1] = 3;
		one.term_map[0] = 2;

		// x^2 + 2x + 1
		auto two = ExprNode("x");
		two.term_map[2] = 1;
		two.term_map[1] = 2;
		two.term_map[0] = 1;

		auto result = one * two;

		// 3x^3 + 8x^2 + 7x + 2
		auto expected = ExprNode("x");
		expected.term_map[3] = 3;
		expected.term_map[2] = 8;
		expected.term_map[1] = 7;
		expected.term_map[0] = 2;

		REQUIRE(result == expected);
	}
}

TEST_CASE("expression divide", "[expression]")
{
	SECTION("divide by zero")
	{
		auto one = ExprNode(1);
		auto two = ExprNode(0);
		REQUIRE_THROWS(one / two);
	}
	SECTION("integer")
	{
		auto one = ExprNode(1);
		auto two = ExprNode(2);
		auto result = one / two;
		auto expected = ExprNode(0.5);
		REQUIRE(result == expected);
	}
	/*
	SECTION("expression")
	{
		// 3x + 2
		auto one = ExprNode("x");
		one.term_map[1] = 3;
		one.term_map[0] = 2;

		// x^2 + 2x + 1
		auto two = ExprNode("x");
		two.term_map[2] = 1;
		two.term_map[1] = 2;
		two.term_map[0] = 1;

		auto result = one * two;

		// 3x^3 + 8x^2 + 7x + 2
		auto expected = ExprNode("x");
		expected.term_map[3] = 3;
		expected.term_map[2] = 8;
		expected.term_map[1] = 7;
		expected.term_map[0] = 2;

		REQUIRE(result == expected);
	}
	*/
}