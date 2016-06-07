#include "int48_t.hpp"

#include <iostream>

template <typename T, typename S, typename U>
void test_equal(T a, S b, U c) {
    if (!(a == b)) {
        std::cout << "FAIL !(a == b): a=" << a << " b=" << b << " state=" << c << std::endl;
        exit(1);
    }
    if (!(b == a)) {
        std::cout << "FAIL !(b == a): a=" << a << " b=" << b << " state=" << c << std::endl;
        exit(1);
    }
    if (a != b) {
        std::cout << "FAIL a != b: a=" << a << " b=" << b << " state=" << c << std::endl;
        exit(1);
    }
    if (b != a) {
        std::cout << "FAIL b != a: a=" << a << " b=" << b << " state=" << c << std::endl;
        exit(1);
    }
}

void run_complete_test(std::int64_t i) {
    int48_t x0(i);
    std::int64_t x1 = x0;
    test_equal(i, x1, x0);
    test_equal(x0, x1, x0);

    int48_t x2(x0);
    test_equal(x0, x2, i);
}

constexpr std::int64_t M = 65536l;
constexpr std::int64_t M2 = M * M;
constexpr std::int64_t M_minus = 65535l;
constexpr std::int64_t M_2minus = 65534l;
constexpr std::int64_t M_half = 32768l;

int main() {
    // positive
    for (std::int64_t i = 0; i < M; ++i) {
        run_complete_test(i);
    }
    for (std::int64_t i = 0; i < M; ++i) {
        for (std::int64_t j = 0; j < 16; ++j) {
            run_complete_test(i * M + j);
        }
        run_complete_test(i * M + M_minus);
    }
    for (std::int64_t i = 0; i < M_half; ++i) {
        for (std::int64_t j = 0; j < 16; ++j) {
            for (std::int64_t k = 0; k < 16; ++k) {
                run_complete_test(i * M2 + j * M + k);
            }
            run_complete_test(i * M2 + M * M_minus);
        }
        run_complete_test(i * M * M2 * M + M_minus);
    }

    // negative
    for (std::int64_t i = 0; i < M_minus; ++i) {
        run_complete_test(-i);
    }
    for (std::int64_t i = 0; i < M; ++i) {
        for (std::int64_t j = 0; j < 16; ++j) {
            run_complete_test(-(i * M + j));
        }
        run_complete_test(-(i * M + M_2minus));
    }
    for (std::int64_t i = 0; i < M_half; ++i) {
        for (std::int64_t j = 0; j < 16; ++j) {
            for (std::int64_t k = 0; k < 16; ++k) {
                run_complete_test(-(i * M2 + j * M + k));
            }
            run_complete_test(-(i * M2 + M * M_2minus));
        }
        run_complete_test(-(i * M * M2 * M + M_2minus));
    }
}
