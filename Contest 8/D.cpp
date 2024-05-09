#include <iostream>
#include <vector>

static const long long prime_pow = 1000000007;

template<typename T>
T trueMod(T val) {
    return ((val % prime_pow) + prime_pow) % prime_pow;
}

template<typename T>
T mPow(T val, long long pow) {
    T result = 1;

    while (pow) {
        // std::cout << pow << '\n';
        if (pow & 1) {
            result = (result * val) % prime_pow;
        }

        val = (val * val) % prime_pow;
        pow >>= 1;
    }

    return result;
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

template<typename T>
T divMod(T val1, T val2) {
    return mul(val1, mPow(val2, prime_pow - 2));
}

template<typename T>
T nod(T val1, T val2) {
    if (val1 < val2) std::swap(val1, val2);

    while (val1 % val2 != 0) {
        val1 %= val2;
        std::swap(val1, val2);
    }

    return val2;
}

void precalcFactorials(std::vector<long long>& factorial) {
    size_t arr_size = factorial.size();
    for (size_t i = 1; i < arr_size; ++i) {
        factorial[i] = trueMod(factorial[i - 1] * i);
    }
}

long long approximate(const long long n, const long long k, const std::vector<long long>& factorial) {
    size_t arr_size = factorial.size();

    long long tmp = 0, ret_data = 0;
    for (size_t i = 1; i < arr_size; ++i) {
        tmp = mPow(i, n) * divMod(divMod(factorial[k], factorial[i]), factorial[k - i]);
        tmp = (i + k) & 1 ? tmp * -1 : tmp;

        ret_data = trueMod(ret_data + trueMod(tmp));
    }

    return divMod(ret_data, static_cast<long long>(factorial[k]));
}

int main() {
   long long n = 0, k = 0;
   std::vector<long long> weight, factorial;

   std::cin >> n >> k;

   weight   .resize(n    , 0);
   factorial.resize(k + 1, 1);

   for (long long i = 0; i < n; ++i) {
       std::cin >> weight[i];
   }

   long long w_cnt = 0;
   for (const auto val : weight) {
      w_cnt = trueMod(w_cnt + val);
   }

   precalcFactorials(factorial);
   long long sub_res = trueMod(mul(n - 1, approximate(n - 1, k, factorial)) + approximate(n, k, factorial));
   long long result  = mul(w_cnt, sub_res);
   std::cout << result << '\n';

   return 0; 
}