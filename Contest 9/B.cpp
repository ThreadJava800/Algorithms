#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

static const long double kEpsilon = 1e-12;

struct Point {
    Point()
        :  x (0),
           y (0) 
    {}

    explicit Point(const long double _x, const long double _y)
        :  x (_x),
           y (_y)
    {}

    friend std::istream& operator>> (std::istream& stream, Point& point) {
        stream >> point.x >> point.y;
        return stream;    
    }

    friend std::ostream& operator<< (std::ostream& stream, const Point& point) {
        stream << point.x << ' ' << point.y << '\n';
        return stream;    
    }

    friend bool operator<(const Point& lhs, const Point& rhs) {
        return lhs.x < rhs.x;
    }

    friend bool operator==(const Point& lhs, const Point& rhs) {
        return std::abs(lhs.x - rhs.x) < kEpsilon;
    }

    long double x = 0;
    long double y = 0;
};

bool isPossible(const std::vector<Point>& points) {
    const auto max_x = std::max_element(points.begin(), points.end(), [](const Point &lhs, const Point &rhs) {
        return lhs.x < rhs.x;
    });
    const auto min_x = std::max_element(points.begin(), points.end(), [](const Point &lhs, const Point &rhs) {
        return lhs.x > rhs.x;
    });

    const long double axis_x = min_x->x / 2 + max_x->x / 2;
    std::unordered_map<long double, std::unordered_map<long double, int>> point_map;

    for (const auto point : points) {
        const long double delta = point.x - axis_x;

        if (delta < 0) {
            point_map[-1 * delta][point.y]--;
        } else if (delta > 0) {
            point_map[     delta][point.y]++;
        }
    }

    for (const auto [x, mappa] : point_map) {
        for (const auto [y, cnt] : mappa) {
            if (cnt != 0) return false;
        }
    }

    return true;
}

int main() {
    unsigned n = 0;
    std::vector<Point> points;

    std::cin >> n;
    points.resize(n);

    for (unsigned i = 0; i < n; ++i) {
        std::cin >> points[i];
    }

    const bool result = isPossible(points);
    if (result) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}