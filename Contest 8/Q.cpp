#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>

#include <limits>


const unsigned MOD = 1'000'000'000 + 7;

unsigned trueMod(long long val)
{
    return ((val % MOD) + MOD) % MOD;
}

unsigned mul(unsigned val1, unsigned val2)  { 
    unsigned return_val = 0;
    val1 %= MOD; 
  
    while (val2 > 0) { 
        if (val2 & 1) 
            return_val = (return_val + val1) % MOD; 
        val1 = (val1 * 2) % MOD; 
        val2 >>= 1;
    } 
  
    return return_val; 
} 

int main() {
    size_t N = 0;
    std::cin >> N;

    std::vector<unsigned> values(N, 0);
    std::vector<std::bitset<31>> bitset_arrays;

    for (size_t i = 0; i < N; ++i) {
        std::cin >> values[i];
        bitset_arrays.emplace_back(values[i]);
    }


    size_t result = 0;
    for (size_t i = 0; i < 31; ++i) {
        size_t cnt_ones = 0;
        size_t cnt_zero = 0;

        for (const auto bitset : bitset_arrays) {
            if (bitset[i] == 1) cnt_ones++;
            else                cnt_zero++;
        }

        if (i == 0) {
            result += mul(cnt_ones, cnt_zero);
        } else {
            result += mul(mul(cnt_ones, cnt_zero), (2 << (i - 1)));
        }
    }

    std::cout << trueMod(result) << '\n';

    return 0;
}