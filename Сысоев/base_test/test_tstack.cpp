#include "stack.h"
#include <gtest.h>

TEST(Stack, can_create_stack_with_positive_length)
{
  ASSERT_NO_THROW(Stack<int> st(5));
}
