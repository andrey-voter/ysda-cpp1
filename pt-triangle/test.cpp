#include <point_triangle.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple") {
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{4, 0}, Point{2, 4}}, Point{2, 2}) == true);
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{4, 0}, Point{2, 4}}, Point{2, 0}) == true);
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{4, 0}, Point{2, 4}}, Point{3, 3}) == false);
    CHECK(IsPointInTriangle(Triangle{Point{-2, -2}, Point{-6, -2}, Point{-4, -6}}, Point{-3, -3}) == true);
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{4, 0}, Point{2, 4}}, Point{1, 1}) == true);
}

TEST_CASE("Degenerated") {
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{0, 0}, Point{0, 0}}, Point{0, 0}) == true);
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{1, 1}, Point{3, 3}}, Point{2, 2}) == true);
    CHECK(IsPointInTriangle(Triangle{Point{0, 0}, Point{1, 1}, Point{3, 3}}, Point{4, 4}) == false);
}

