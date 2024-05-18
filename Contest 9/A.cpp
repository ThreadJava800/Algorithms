#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <utility>

static long double kEpsilon = 1e-12;

struct Point;
using Line = std::pair<Point, Point>;

struct Point {
    long double x = 0;
    long double y = 0;

    friend std::istream& operator>> (std::istream& stream, Point& point) {
        stream >> point.x >> point.y;
        return stream;    
    }
};

bool compareDoubles(long double val1, long double val2) {
    return std::abs(val1 - val2) < kEpsilon;
}

long double getDistance(Point p1, Point p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));    
}

enum INTERSECTION_TYPE {
    NONE,
    POINT,
    SAME
};

INTERSECTION_TYPE getIntersLineLine(Point lineStart1, Point lineEnd1, Point lineStart2, Point lineEnd2, Point &intersPoint) {
    // build 1 segment a1x + b1y = c1
    double a1 = lineEnd1.y        - lineStart1.y;
    double b1 = lineStart1.x      - lineEnd1.x;
    double c1 = lineStart1.x * a1 + lineStart1.y * b1;

    // build 2 segment a2x + b2y = c2
    double a2 = lineEnd2.y        - lineStart2.y;
    double b2 = lineStart2.x      - lineEnd2.x;
    double c2 = lineStart2.x * a2 + lineStart2.y * b2;

    // x = (c1*b2 - c2*b1) / (a1*b2 - a2*b1)
    // y = (a1*c2 - a2*c1) / (a1*b2 - a2*b1)
    double delta = a1 * b2 - a2 * b1;

    // parallel
    if (fabs(delta) < kEpsilon) {
        if (lineStart1.x == lineStart2.x && lineStart1.y == lineStart2.y &&
            lineEnd1  .x == lineEnd2  .x && lineEnd1  .y == lineEnd2  .y)
            return SAME;

        return NONE;
    }

    double x = (c1 * b2 - c2 * b1) / delta;
    double y = (c2 * a1 - c1 * a2) / delta;

    intersPoint = {x, y};

    // figure out that point is inside segment
    if ((std::min(lineStart1.x, lineEnd1.x) <= x && x <= std::max(lineStart1.x, lineEnd1.x) &&
         std::min(lineStart1.y, lineEnd1.y) <= y && y <= std::max(lineStart1.y, lineEnd1.y)) &&
       ( std::min(lineStart2.x, lineEnd2.x) <= x && x <= std::max(lineStart2.x, lineEnd2.x)  &&
         std::min(lineStart2.y, lineEnd2.y) <= y && y <= std::max(lineStart2.y, lineEnd2.y)))
        return POINT;

    return NONE;
}

Point getNormal(Line line, Point p) {
    long double x1 = line.first.x , y1 = line.first.y;
    long double x2 = line.second.x, y2 = line.second.y;
    long double x0 = p.x, y0 = p.y;

    long double a = y1 - y2;
    long double b = x2 - x1;
    long double c = (x1 - x2) * y1 + (y2 - y1) * x1;

    long double x = (b * (b * x0 - a * y0) - a * c) / (a * a + b * b);
    long double y = (a * (-b * x0 + a * y0) - b * c) / (a * a + b * b);

    return {x, y};
}

bool pointInsideSegment(Line line, Point point) {
    return compareDoubles(getDistance(line.first, point) + getDistance(line.second, point), getDistance(line.first, line.second));
}

long double getLen(Line line) {
    long double x0 = line.first.x, x1 = line.second.x;
    long double y0 = line.first.y, y1 = line.second.y;

    return std::sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

long double getDistance(Line line1, Line line2) {
    const auto is_zero_1 = compareDoubles(getLen(line1), 0.0l);
    const auto is_zero_2 = compareDoubles(getLen(line2), 0.0l);

    if (is_zero_1 && is_zero_2) {
        return getLen({line1.first, line2.first});
    }

    Point test;
    auto res = getIntersLineLine(line1.first, line1.second, line2.first, line2.second, test);
    if (res == POINT || res == SAME) return 0.0l;
    long double min_1 = __INT_MAX__, min_2 = __INT_MAX__, min_3 = __INT_MAX__, min_4 = __INT_MAX__;

    if (is_zero_1 && !is_zero_2 || !is_zero_1 && !is_zero_2) {
        Point norm1 = getNormal(line2, line1.first);

        if (pointInsideSegment(line2, norm1)) {
            min_1 = getDistance(norm1, line1.first);
        } else {
            min_1 = std::min(getDistance(line1.first,  line2.first), getDistance(line1.first , line2.second));
        }
    }
    
    if (!is_zero_1 && is_zero_2 || !is_zero_1 && !is_zero_2) {
        Point norm1 = getNormal(line1, line2.first);

        if (pointInsideSegment(line1, norm1)) {
            min_2 = getDistance(norm1, line2.first);
        } else {
            min_2 = std::min(getDistance(line1.first,  line2.first), getDistance(line1.second , line2.first));
        }
    }

    if (!is_zero_1 && !is_zero_2) {
        Point norm1 = getNormal(line2, line1.second);

        if (pointInsideSegment(line2, norm1)) {
            min_3 = getDistance(norm1, line1.second);
        } else {
            min_3 = std::min(getDistance(line1.second,  line2.first), getDistance(line1.second , line2.second));
        }
    }

    if (!is_zero_1 && !is_zero_2) {
        Point norm1 = getNormal(line1, line2.first);

        if (pointInsideSegment(line1, norm1)) {
            min_2 = getDistance(norm1, line2.first);
        } else {
            min_2 = std::min(getDistance(line1.first,  line2.second), getDistance(line1.second , line2.second));
        }
    }

    return std::min(std::min(min_1, min_2), std::min(min_3, min_4));
}

int main() {
    Line first_line, second_line;

    std::cin >> first_line .first >> first_line .second;
    std::cin >> second_line.first >> second_line.second;


    std::cout << std::fixed << std::setw(16) << std::setprecision(16) << getDistance(first_line, second_line) << '\n';

    return 0;
}