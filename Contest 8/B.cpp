#include <cmath>
#include <iostream>

int main()
{
    int val = 0;
    std::cin >> val;

    size_t count = 0;
    for (int i = 3; i <= val; ++i)
    {
        for (int j = 2; j <= sqrt(i) + 1; ++j)
        {
            if (i % j == 0)
            {
                count += j;
                break;
            }
        }
    }

    std::cout << count << '\n';

    return 0;
}