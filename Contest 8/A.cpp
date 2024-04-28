#include <iostream>

static const int prime_pow = 1000000007;

long long trueMod(long long val)
{
    return ((val % prime_pow) + prime_pow) % prime_pow;
}

long long mPow(long long val, int pow)
{
    long long result = 1;

    while (pow)
    {
        // std::cout << pow << '\n';
        if (pow & 1)
        {
            result = (result * val) % prime_pow;
        }

        val = (val * val) % prime_pow;
        pow >>= 1;
    }

    return result;
}

int main()
{
    long long a = 0, b = 0, c = 0, d = 0;
    std::cin >> a >> b >> c >> d;

    long long first_val  = trueMod(trueMod(a) * mPow(b, prime_pow - 2));
    long long second_val = trueMod(trueMod(c) * mPow(d, prime_pow - 2));
    long long result     = trueMod(first_val + second_val);

    std::cout << result << '\n';

    return 0;
}