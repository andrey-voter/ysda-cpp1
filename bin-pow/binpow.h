#pragma once

#include <stdexcept>

int BinPow(int a, int64_t b, int c) {
    int result = 1;
    a %= c;
    while (b > 0) {
        if (b % 2) {
            result = (1LL * result * a) % c;
        }
        a = (1LL * a * a) % c;
        b /= 2;
    }
    return (result + c) % c;
}
