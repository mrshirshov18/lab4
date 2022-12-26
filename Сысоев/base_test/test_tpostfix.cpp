
#include "postfix.h"
#include <gtest.h>

TEST(TPostfix, can_parse_negative_number) {
    string str = "-3+1";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_number_with_point) {
    string str = "-3.2+1/5.6";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_long_number) {
    string str = "-345+1156.4-876";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_variables) {
    string str = "s-v/h";
    TPostfix expression(str);
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_long_variables) {
    string str = "first_argument/second_arfument-third_argument";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_variables_and_numbers) {
    string str = "first_argument/second_arfument-third_argument/2-187+18";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_parse_variables_with_numbers) {
    string str = "a1/a2-a3";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_when_empty_string) {
    string str = "";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_when_empty_brackets) {
    string str = " ()+5";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_brackets_expression_1) {
    string str = " (2+3)";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, can_parse_brackets_expression_2) {
    string str = " (2+3)-5*(16+(43-8)*(-1))";
    ASSERT_NO_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_parse_with_wrong_brackets_expression) {
    string str = " (2+3)-5*(16+(43-8)*(-1))))";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_parse_expression_with_wrong_number_points) {
    string str = "..5+6";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_parse_expression_with_wrong_number_point_minus) {
    string str = "-.5+6";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_expression_with_wrong_operators) {
    string str = "8+-6";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_expression_with_lost_operators) {
    string str = "8-6h-7";
    ASSERT_ANY_THROW(TPostfix expression(str));
}
TEST(TPostfix, throws_when_invalid_symbols) {
    string str = "a+6&7";
    ASSERT_ANY_THROW(TPostfix expression(str));
}

TEST(TPostfix, can_get_infix)
{
    TPostfix str("abacab-18*(6*7)-156");
    EXPECT_EQ("abacab-18*(6*7)-156", str.GetInfix());
}

TEST(TPostfix, can_get_postfix)
{
    TPostfix expression("12*((3+4*5)-7)");
    EXPECT_EQ(" 12 3 4 5 * + 7 - *", expression.GetPostfix());
}
TEST(TPostfix, can_get_postfix_2)
{
    TPostfix expression("2*x+3");
    EXPECT_EQ(" 2 x * 3 +", expression.GetPostfix());
}


TEST(TPostfix, can_calculate_expression_with_numbers)
{
    TPostfix expression("12*((3+4*5)-7)");
    ostream output(nullptr);
    double result = expression.Calculate(cin, output);
    double expected = 12 * ((3 + 4 * 5) - 7);
    EXPECT_EQ(expected, result);
}

TEST(TPostfix, can_calculate_expression_with_numbers_and_variables)
{
    TPostfix expression(" abacaba-18*(6*7)-156");
    istringstream values(" 1");
    ostream output(nullptr);
    double result = expression.Calculate(values, output);
    double expected = 1 - 18 * (6 * 7) - 156;

    EXPECT_EQ(expected, result);
}

