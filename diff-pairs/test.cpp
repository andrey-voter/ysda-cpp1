#include <diff_pairs.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple test") {
    CHECK(CountPairs({1, 2, 2, 3, 4}, 4) == 2);
    CHECK(CountPairs({1, 2, 2, 3, 4, 2}, 4) == 4);
    CHECK(CountPairs({}, 0) == 0);
}
