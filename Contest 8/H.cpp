#include <iostream>
#include <vector>

static const long long prime_pow = 1000000009;

template<typename T>
T trueMod(T val) {
    return ((val % prime_pow) + prime_pow) % prime_pow;
}

template<typename T>
T mul(T val1, T val2)  { 
    T return_val = 0;
    val1 %= prime_pow; 
  
    while (val2 > 0) { 
        if (val2 & 1) 
            return_val = (return_val + val1) % prime_pow; 
        val1 = (val1 * 2) % prime_pow; 
        val2 >>= 1;
    } 
  
    return return_val; 
} 

struct AirportTable {
    long long x;
    long long y;
    long long flight_cnt;
};

struct Director {
    long long up;
    long long right;
    long long down;
    long long left;

    Director operator*(Director const& other)
    {
        Director ret_data;

        ret_data.up    = trueMod(mul(up  , other.up   ) + mul(right, other.down ));
        ret_data.right = trueMod(mul(up  , other.right) + mul(right, other.left ));
        ret_data.down  = trueMod(mul(down, other.up   ) + mul(left , other.down ));
        ret_data.left  = trueMod(mul(down, other.right) + mul(left , other.left ));

        return ret_data;
    }
};

Director mPow(Director directions, long long val2) {
    if (val2 <= 1) return directions;

    Director powed = mPow(directions, val2 / 2);

    return val2 & 1 ? (powed * powed) * directions : (powed * powed);
}

long long calculatePathCnt(std::vector<AirportTable>& airports, const long long n, const long long q) {
    long long result = 0;

    Director tmp;
    for (int i = 0; i < n; ++i) {
        long long differ = airports[i].x + q;

        if (airports[i].y == differ + 1 || airports[i].y == differ) {
            result = trueMod(airports[i].flight_cnt + result);
        } else if (differ < airports[i].y) {
            tmp = {1, 1, 1, 0};

            Director powed = mPow(tmp, airports[i].y - 1 - differ);
            long long mul_res = mul(airports[i].flight_cnt, trueMod(powed.up + powed.right));
            result = trueMod(mul_res + result);
        }
    }

    return result;
}

int main() {
    int n = 0, q = 0;
    std::vector<AirportTable> airports;
    std::vector<long long>    diagonals;

    std::cin >> n >> q;
    airports .resize(n);
    diagonals.resize(q);

    for (int i = 0; i < n; ++i) {
        std::cin >> airports[i].x >> airports[i].y >> airports[i].flight_cnt;
    }
    for (int i = 0; i < q; ++i) {
        std::cin >> diagonals[i];
    }

    // preparation finished

    for (const auto diagonal : diagonals) {
        std::cout << calculatePathCnt(airports, n, diagonal) << '\n';
    }

    return 0;
}