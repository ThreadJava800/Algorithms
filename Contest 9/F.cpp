#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

struct Point {
    int x;
    int y;

    // Point(const Point& other) {

    // }

    friend bool operator== (const Point& lhs, const Point& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend std::istream& operator>> (std::istream& stream, Point& point) {
        stream >> point.x >> point.y;
        return stream;    
    }

    static double Distance2(const Point p1, const Point p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    }
};

struct PointHash {
    size_t operator() (Point const &v) const {
        return std::hash<int>{}(v.x * v.y);    
    }
};

std::pair<Point, Point> getDiagonal(const Point p1, const Point p2, bool& is_present) {
    const double centre_x = static_cast<double>(p1.x + p2.x) / 2.0;
    const double centre_y = static_cast<double>(p1.y + p2.y) / 2.0;

    const double p3_x = centre_x - (static_cast<double>(p1.y) - centre_y);
    const double p3_y = centre_y + (static_cast<double>(p1.x) - centre_x);

    const double p4_x = centre_x - (static_cast<double>(p2.y) - centre_y);
    const double p4_y = centre_y + (static_cast<double>(p2.x) - centre_x);

    double int_part;
    if (std::modf(p3_x, &int_part) != 0 || std::modf(p3_y, &int_part) != 0 || std::modf(p4_x, &int_part) != 0 || std::modf(p4_y, &int_part) != 0) {
        is_present = false;
        return {};
    }

    is_present = true;
    return {{static_cast<int>(p3_x), static_cast<int>(p3_y)}, {static_cast<int>(p4_x), static_cast<int>(p4_y)}};
}

unsigned countSquares(const std::vector<Point>& points) {
    unsigned result    = 0;
    size_t   point_cnt = points.size();

    std::unordered_set<Point, PointHash> point_set;
    for (const auto p : points) {
        point_set.insert(p);
    }

    for (size_t i = 0; i < point_cnt; ++i) {
        for (size_t j = i + 1; j < point_cnt; ++j) {
            if (i == j) continue;

            bool is_present = false;
            const auto diagonal = getDiagonal(points[i], points[j], is_present);

            if (!is_present) continue;

            if (point_set.contains(diagonal.first) && point_set.contains(diagonal.second)) {
                result++;
            }
        }
    }

    return result;
}

int main() {
    size_t N = 0;
    std::vector<Point> modules;

    std::cin >> N;

    modules.resize(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> modules[i];
    }

    std::cout << countSquares(modules) / 2 << '\n';

    return 0;
}