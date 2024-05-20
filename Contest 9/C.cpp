#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <stack>
#include <set>
#include <vector>

static long double kEpsilon = 1e-25;

bool compareDoubles(long double val1, long double val2) {
    return std::abs(val1 - val2) < kEpsilon;
}

struct Point {
public:

    enum Orientation {
        COLLINEAR,
        CLOCK,
        COUNTER_CLOCK
    };

    Point()
      : x (0),
        y (0)
    {}

    explicit Point(const int x, const int y)
      : x (x),
        y (y)
    {}

    friend std::istream& operator>> (std::istream& stream, Point& point) {
        stream >> point.x >> point.y;
        return stream;    
    }

    friend std::ostream& operator<< (std::ostream& stream, const Point& point) {
        stream << std::fixed << std::setprecision(0) << point.x << ' ' << std::fixed << std::setprecision(0) << point.y << '\n';
        return stream;    
    }

    static Orientation getOrientation(const Point p1, const Point p2, const Point p3) {
        long double expr = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);

        if (compareDoubles(expr, 0)) return COLLINEAR;
        if (expr >  0) return CLOCK;
        return COUNTER_CLOCK;
    } 

    static long double getDistance(const Point p1, const Point p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    }

    bool operator== (const Point other) {
        return compareDoubles(x, other.x) && compareDoubles(y, other.y);
    }

    long double getAngle(const Point centre = Point(0, 0)) const {
        long double dx = x - centre.x;
        long double dy = y - centre.y;

        return -1 * std::atan2(dx, -dy);
    }

public:
    long double x;
    long double y;
};


Point p0;
int comparator(const void* _p1, const void* _p2) {
    Point* p1 = (Point*) _p1;
    Point* p2 = (Point*) _p2;

    Point::Orientation orient = Point::getOrientation(p0, *p1, *p2);
    if (orient == Point::COLLINEAR) {
        const auto p0_p2 = Point::getDistance(p0, *p2);
        const auto p0_p1 = Point::getDistance(p0, *p1);

        if (p0_p2 < p0_p1) return 1;
        else               return -1;
    }
    if (orient == Point::CLOCK) return 1;
    return -1;
}

class ConvexHull {
public:

    ConvexHull()
      : all_points    (),
        border_points ()
    {}

    friend std::istream& operator>> (std::istream& stream, ConvexHull& hull) {
        int N = 0;
        stream >> N;

        hull.all_points.resize(N);

        for (int i = 0; i < N; ++i) {
            stream >> hull.all_points[i];
        }

        return stream;    
    }

    friend std::ostream& operator<< (std::ostream& stream, const ConvexHull& hull) {
        const size_t point_cnt = hull.border_points.size();
        
        stream << std::fixed << point_cnt << '\n';

        for (size_t i = 0; i < point_cnt; ++i) {
            stream << hull.border_points[i];
        }

        return stream;    
    }

    void buildConvexHull() {
        size_t p0_ind = getStartPoint(all_points);
        std::swap(all_points[0], all_points[p0_ind]);
        p0 = all_points[0];

        std::qsort(&all_points[1], all_points.size() - 1, sizeof(Point), comparator);

        int    border_points_cnt = 1;
        size_t all_points_cnt    = all_points.size();
        for (int i = 1; i < all_points_cnt; ++i) {
            while (i < all_points_cnt - 1 && Point::getOrientation(p0, all_points[i], all_points[i + 1]) == Point::COLLINEAR) {
                i++;
            }
            
            all_points[border_points_cnt] = all_points[i];
            border_points_cnt++;
        }

        runGraham(border_points_cnt);
        sortBorderPoints();
    }

    long double getSquare() const {
        size_t el_cnt = border_points.size();
        long long result = 0;

        for (size_t i = 0; i < el_cnt - 1; ++i) {
            result += static_cast<size_t>(border_points[i].x * border_points[i + 1].y);
        }
        result += static_cast<size_t>(border_points[el_cnt - 1].x * border_points[0].y);

        for (size_t i = 0; i < el_cnt - 1; ++i) {
            result -= static_cast<size_t>(border_points[i + 1].x * border_points[i].y);
        }
        result -= static_cast<size_t>(border_points[0].x * border_points[el_cnt - 1].y);

        return static_cast<long double>(std::abs(result)) / 2.0l;
    }

private:

    void sortBorderPoints() {
        const size_t p0_ind = getStartPoint(border_points);
        const Point  p0     = border_points[p0_ind];

        Point centre;

        const size_t border_point_cnt = border_points.size();
        for (size_t i = 0; i < border_point_cnt; ++i) {
            centre.x += (static_cast<long double>(border_points[i].x) / static_cast<long double>(border_point_cnt));
            centre.y += (static_cast<long double>(border_points[i].y) / static_cast<long double>(border_point_cnt));
        }

        std::sort(border_points.begin(), border_points.end(), [centre](const Point p1, const Point p2) {
            return p1.getAngle(centre) < p2.getAngle(centre);
        });

        const auto p0_it = std::find(border_points.begin(), border_points.end(), p0);
        std::rotate(border_points.begin(), p0_it, border_points.end());
    }

    size_t getStartPoint(const std::vector<Point>& search_vec) const {
        Point min_point = search_vec[0];
        int   min_ind   = 0;

        const size_t all_point_cnt = search_vec.size();
        for (size_t i = 0; i < all_point_cnt; ++i) {
            const auto point = search_vec[i];
            if (compareDoubles(point.y, min_point.y) && point.x < min_point.x 
             || point.y < min_point.y) {
                min_point = point;
                min_ind   = i;
            }
        }

        return min_ind;
    }

    Point getSecond(std::stack<Point>& point_stack) const {
        Point first  = point_stack.top(); point_stack.pop();
        Point second = point_stack.top();

        point_stack.push(first);

        return second; 
    }

    void runGraham(size_t border_point_cnt) {
        std::stack<Point> point_stack;

        point_stack.push(all_points[0]);
        point_stack.push(all_points[1]);
        point_stack.push(all_points[2]);

        for (size_t i = 3; i < border_point_cnt; ++i) {
            while (point_stack.size() > 1 && 
                   Point::getOrientation(getSecond(point_stack), point_stack.top(), all_points[i]) != Point::COUNTER_CLOCK) {
                point_stack.pop();
            }
            
            point_stack.push(all_points[i]);
        }

        while (!point_stack.empty()) {
            border_points.push_back(point_stack.top()); point_stack.pop();
        }
    }

private:
    std::vector<Point> all_points;
    std::vector<Point> border_points;
};

int main() {
    ConvexHull convex_hull;
    std::cin >> convex_hull;

    convex_hull.buildConvexHull();
    std::cout << convex_hull;

    std::cout << std::fixed << std::setprecision(1) << convex_hull.getSquare() << '\n';

    return 0;
}