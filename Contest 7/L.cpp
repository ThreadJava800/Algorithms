#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class PalindromeCounter {
public:
    explicit PalindromeCounter(const std::string_view _str)
        : str(_str) {
        odd_palindromes .resize(str.length(), 0);
        even_palindromes.resize(str.length(), 0);
    }

    size_t getPalindromeCnt() {
        countOddPalindromes ();
        countEvenPalindromes();

        size_t ans = 0;
        for (int i = 0; i < str.length(); i++) {
            ans += (odd_palindromes[i] + even_palindromes[i]);
            // std::cout << i << ' ' << ans << ' ' << odd_palindromes[i] << ' ' << even_palindromes[i] << '\n';
        }

        return ans;
    }

private:

    void countOddPalindromes() {
        int left_bound = 0, right_bound = -1;
        int str_len = static_cast<int>(str.length());

        for (int i = 0; i < str_len; i++) {
            int palindrome_len = 1;

            if (i <= right_bound) 
                palindrome_len = std::min(right_bound - i, odd_palindromes[left_bound + right_bound - i]) + 1;

            while (
                    (i + palindrome_len < str_len) && 
                    (i - palindrome_len >= 0)      &&
                    str[i + palindrome_len] == str[i - palindrome_len]
                  ) {
                palindrome_len++;
            }

            odd_palindromes[i] = --palindrome_len;

            if (i + palindrome_len > right_bound) {
                left_bound  = i - palindrome_len;
                right_bound = i + palindrome_len;
            }
        }
    }

    void countEvenPalindromes() {
        int left_bound = 0, right_bound = -1;
        int str_len = static_cast<int>(str.length());
    
        for (int i = 0; i < str_len; i++) {
            int palindrome_len = 1;

            if (i <= right_bound) 
                palindrome_len = std::min(right_bound - i + 1, even_palindromes[right_bound - i + left_bound + 1]) + 1;

            while (
                    (i + palindrome_len - 1 < str_len) && 
                    (i - palindrome_len >= 0)   &&
                    str[i + palindrome_len - 1] == str[i - palindrome_len]
                  ) {
                palindrome_len++;
            }

            even_palindromes[i] = --palindrome_len;

            if (i + palindrome_len - 1 > right_bound) {
                left_bound  = i - palindrome_len;
                right_bound = i + palindrome_len - 1;
            }
        }
    }

private:
    const std::string_view str;
          std::vector<int> odd_palindromes;
          std::vector<int> even_palindromes;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);

    std::string str;
    std::cin >> str;

    PalindromeCounter pal_cnt(str);
    std::cout << pal_cnt.getPalindromeCnt() << '\n';

    return 0;
}