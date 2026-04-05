#ifndef CONVEX_HULL_UTILS_H
#define CONVEX_HULL_UTILS_H

#include <algorithm>
#include <iostream>
#include <vector>

struct Point {
    int x;
    int y;
};

inline bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

inline void push_front(std::vector<Point>& points, const Point& point) {
    points.insert(points.begin(), point);
}

inline void printPoints(const std::vector<Point>& points) {
    const int n = int(points.size()) - 1;
    std::cout << "[";
    for (int i = 1; i <= n; ++i) {
        std::cout << points[i];
        if (i < n) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

inline bool is_sorted_by_x(const std::vector<Point>& points) {
    return std::is_sorted(
        points.begin() + 1,
        points.end(),
        [](const Point& lhs, const Point& rhs) {
            return lhs.x < rhs.x;
        }
    );
}

inline bool have_distinct_x_and_y(const std::vector<Point>& points) {
    const int n = int(points.size()) - 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (points[i].x == points[j].x || points[i].y == points[j].y) {
                return false;
            }
        }
    }
    return true;
}

inline long long orientation(const Point& a, const Point& b, const Point& c) {
    const long long abx = b.x - a.x;
    const long long aby = b.y - a.y;
    const long long acx = c.x - a.x;
    const long long acy = c.y - a.y;
    return abx * acy - aby * acx;
}

inline bool has_no_three_collinear_points(const std::vector<Point>& points) {
    const int n = int(points.size()) - 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            for (int k = j + 1; k <= n; ++k) {
                if (orientation(points[i], points[j], points[k]) == 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

inline bool is_upper_tangent(const std::vector<Point>& UH1, const std::vector<Point>& UH2, int i, int j) {
    const int n1 = int(UH1.size()) - 1;
    const int n2 = int(UH2.size()) - 1;

    for (int t = 1; t <= n1; ++t) {
        if (t == i) continue;
        if (orientation(UH1[i], UH2[j], UH1[t]) > 0) {
            return false;
        }
    }

    for (int t = 1; t <= n2; ++t) {
        if (t == j) continue;
        if (orientation(UH1[i], UH2[j], UH2[t]) > 0) {
            return false;
        }
    }

    return true;
}

inline std::vector<Point> subarray(const std::vector<Point>& points, int left, int right) {
    std::vector<Point> result(1);
    for (int i = left; i <= right; ++i) {
        result.push_back(points[i]);
    }
    return result;
}

#endif // CONVEX_HULL_UTILS_H
