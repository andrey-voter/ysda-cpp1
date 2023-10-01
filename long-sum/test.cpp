#include <long_sum.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple test") {
    CHECK(LongSum("2", "2") == "4");
    CHECK(LongSum("0", "1") == "1");
    CHECK(LongSum("0", "0") == "0");
    CHECK(LongSum("111", "111") == "222");
    CHECK(LongSum("123", "456") == "579");
    CHECK(LongSum("999", "999") == "1998");

}

TEST_CASE("Different sizes test") {
    CHECK(LongSum("237451699741", "2227893") == "237453927634");
    CHECK(LongSum("7135469354213", "78876397517954397") == "78883532987308610");
    CHECK(LongSum("15863546790182703", "752143002251372444") == "768006549041555147");

}
