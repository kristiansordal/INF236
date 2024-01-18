#include <gtest/gtest.h>

extern "C" {
#include "week1.h"
}

TEST(SwapTest, SwapsValues) {
    int a = 1;
    int b = 2;

    swap(&a, &b);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 1);
}

TEST(SwapTest, SwapsDates) {
    Date a = {1, 1, 2000};
    Date b = {2, 2, 1999};

    swap_date(&a, &b);

    EXPECT_EQ(a.day, 2);
    EXPECT_EQ(a.month, 2);
    EXPECT_EQ(a.year, 1999);
    EXPECT_EQ(b.day, 1);
    EXPECT_EQ(b.month, 1);
    EXPECT_EQ(b.year, 2000);
}

TEST(SortTest, SortsArray) {
    int arr[] = {5, 4, 2, 3, 1};

    int expected[] = {1, 2, 3, 4, 5};
    sort(arr, 5);

    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(expected[i], arr[i]);
    }
}

TEST(SortTest, SortsDates) {
    Date d1 = {1, 1, 2000};
    Date d2 = {2, 2, 1999};
    Date d3 = {3, 3, 1998};
    Date d4 = {4, 4, 1997};
    Date d5 = {5, 5, 1996};
    Date dates[] = {d4, d1, d2, d5, d3};
    Date expected[] = {d5, d4, d3, d2, d1};

    sort_date(dates, 5);

    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(expected[i].day, dates[i].day);
        EXPECT_EQ(expected[i].month, dates[i].month);
        EXPECT_EQ(expected[i].year, dates[i].year);
    }
}

TEST(MatrixMul, MatrixMul) {
    Matrix a;
    Matrix b;

    init_matrix(&a, 9);
    init_matrix(&b, 9);

    Matrix c = mat_mult(&a, &b);

    int expected[] = {21, 28, 38, 28, 38, 53, 38, 53, 77};

    for (int i = 0; i < 9; i++) {
        EXPECT_EQ(expected[i], c.arr[i]);
    }
}
