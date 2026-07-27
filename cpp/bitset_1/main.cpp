#include <iostream>
#include <bitset>

int main() {
    int N, S, P, Q;
    std::cin >> N >> S >> P >> Q;
    constexpr size_t mod = 1U << 31;
    constexpr size_t mask = mod - 1;
    static std::bitset< mod > a;
    size_t val = S; // no need to do modulo on S (constraint 0 <= S < 2^31)
    a[val] = true;
    size_t count = 1;
    for (size_t i = 1; i < N; ++i) {
        // % modulo takes too much time. Here we use a mask (m - 1) 011111... which does the same thing since
        // mod is a power of 2
        val = (val * P + Q) & mask;
        if (!a[val]) {
            count++;
            a[val] = true;
        } else {
            // we got a val we already encountered additional vals will follow the same pattern we have
            // already seen
            break;
        }
    }
    std::cout << count << std::endl;
    return 0;
}
