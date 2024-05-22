#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Flash {
    int    x;
    int    y;
    double angle;

    friend std::istream& operator>> (std::istream& stream, Flash& flash) {
        stream >> flash.x >> flash.y >> flash.angle;
        return stream;    
    }
};

double getAngle(const double x0, const Flash flash, const int l, const int r) {
    double dx    = std::atan(static_cast<double>(x0 - flash.x) / static_cast<double>(flash.y)) + flash.angle;
    double dy    = std::atan(static_cast<double>(r  - flash.x) / static_cast<double>(flash.y));
    double angle = std::tan (std::min(dx, dy)) * flash.y;

    return angle + flash.x;
}

double calculateAnswer(const std::vector<Flash>& flashes, std::vector<double>& dp, const int l, const int r) {
    const size_t dp_size    = dp     .size();
    const size_t flash_size = flashes.size();

    for (size_t i = 0; i < dp_size; ++i) {
        for (size_t j = 0; j < flash_size; ++j) {
            if (!(i & (1 << j))) {
                dp[i ^ (1 << j)] = std::max(dp[i ^ (1 << j)], getAngle(dp[i], flashes[j], l, r));
            }
        }
    }

    return std::min(static_cast<double>(r), dp.back());
}

int main() {
    unsigned n = 0;
    int l = 0, r = 0;
    std::vector<Flash> flashes;
    std::vector<double> dp;

    std::cin >> n >> l >> r;

    r -= l;

    flashes.resize(n);
    dp     .resize(1 << n);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> flashes[i];

        flashes[i].x -= l;
        flashes[i].angle = flashes[i].angle * M_PI / 180.0;
    }

    std::cout << std::fixed << std::setprecision(6) << calculateAnswer(flashes, dp, l, r) << '\n';

    return 0;
}