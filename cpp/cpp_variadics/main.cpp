#include <cmath>
#include <iostream>

template < bool... digits > int reversed_binary_value() {
    const int size = sizeof...(digits);
    int binary[size] = {digits...};
    int res = 0;
    for (int i = 0; i < size; ++i) {
        res += binary[i] * std::pow(2, i);
    }
    return res;
}

template < int n, bool... digits > struct CheckValues {
    static void check(int x, int y) {
        CheckValues< n - 1, 0, digits... >::check(x, y);
        CheckValues< n - 1, 1, digits... >::check(x, y);
    }
};

template < bool... digits > struct CheckValues< 0, digits... > {
    static void check(int x, int y) {
        int z = reversed_binary_value< digits... >();
        std::cout << (z + 64 * y == x);
    }
};

int main() {
    int t;
    std::cin >> t;

    for (int i = 0; i != t; ++i) {
        int x, y;
        std::cin >> x >> y;
        CheckValues< 6 >::check(x, y);
        std::cout << "\n";
    }
}
