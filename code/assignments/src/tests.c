#include "unity.h"

void setUp(void) {
    // Code here will be called before each test.
}

void tearDown(void) {
    // Code here will be called after each test.
}

void test_add_function(void) {
    // Test to ensure the add function correctly adds two numbers
    int result = 5 + 3;
    TEST_ASSERT_EQUAL(8, result);

    // You can add more tests for the same function here
    result = -1 + 1;
    TEST_ASSERT_EQUAL(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_function);
    return UNITY_END();
}
