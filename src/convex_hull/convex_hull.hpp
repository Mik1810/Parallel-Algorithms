#ifndef CONVEX_HULL_UTILS_H
#define CONVEX_HULL_UTILS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
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
    const int n = points.size() - 1;
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
    const int n = points.size() - 1;
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

inline double slope(const Point& a, const Point& b) {
    return double(b.y - a.y) / double(b.x - a.x);
}

inline bool has_no_three_collinear_points(const std::vector<Point>& points) {
    const int n = points.size() - 1;
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
    const int n1 = UH1.size() - 1;
    const int n2 = UH2.size() - 1;

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

inline int Argmax_Y(const std::vector<Point>& UH) {
    const int n = UH.size() - 1;

    if (n == 1) {
        return 1;
    }

    std::vector<int> indices(n - 1);
    std::iota(indices.begin(), indices.end(), 1);

    const auto it = std::partition_point(
        indices.begin(),
        indices.end(),
        [&](int i) {
            return UH[i].y < UH[i + 1].y;
        }
    );

    return (it == indices.end()) ? n : *it;
}

inline int Find_Min_Slope_Point(const std::vector<Point>& UH, int right, const Point& z) {
    if (right == 1) {
        return 1;
    }

    std::vector<int> indices(right - 1);
    std::iota(indices.begin(), indices.end(), 1);

    const auto it = std::partition_point(
        indices.begin(),
        indices.end(),
        [&](int i) {
            return slope(UH[i], z) > slope(UH[i + 1], z);
        }
    );

    return (it == indices.end()) ? right : *it;
}

inline int Find_Max_Slope_Point(const Point& z, const std::vector<Point>& UH, int right) {
    if (right == 1) {
        return 1;
    }

    std::vector<int> indices(right - 1);
    std::iota(indices.begin(), indices.end(), 1);

    const auto it = std::partition_point(
        indices.begin(),
        indices.end(),
        [&](int i) {
            return slope(z, UH[i]) < slope(z, UH[i + 1]);
        }
    );

    return (it == indices.end()) ? right : *it;
}

inline std::vector<Point> subarray(const std::vector<Point>& points, int left, int right) {
    std::vector<Point> result(1);
    for (int i = left; i <= right; ++i) {
        result.push_back(points[i]);
    }
    return result;
}

inline double Canvas_X(const Point& point, double min_x, double max_x, int width, int margin) {
    if (min_x == max_x) {
        return width / 2.0;
    }

    return margin + (point.x - min_x) * (width - 2.0 * margin) / (max_x - min_x);
}

inline double Canvas_Y(const Point& point, double min_y, double max_y, int height, int margin) {
    if (min_y == max_y) {
        return height / 2.0;
    }

    return height - margin - (point.y - min_y) * (height - 2.0 * margin) / (max_y - min_y);
}

inline void Write_SVG(const std::vector<Point>& S, const std::vector<Point>& UH, const std::string& path) {
    const int n = S.size() - 1;
    const int h = UH.size() - 1;
    const int width = 900;
    const int height = 600;
    const int margin = 60;

    int min_x = S[1].x;
    int max_x = S[1].x;
    int min_y = S[1].y;
    int max_y = S[1].y;

    for (int i = 2; i <= n; ++i) {
        min_x = std::min(min_x, S[i].x);
        max_x = std::max(max_x, S[i].x);
        min_y = std::min(min_y, S[i].y);
        max_y = std::max(max_y, S[i].y);
    }

    const double x_padding = std::max(1.0, 0.15 * std::max(1, max_x - min_x));
    const double y_padding = std::max(1.0, 0.15 * std::max(1, max_y - min_y));
    const double plot_min_x = std::min(0.0, min_x - x_padding);
    const double plot_max_x = std::max(0.0, max_x + x_padding);
    const double plot_min_y = std::min(0.0, min_y - y_padding);
    const double plot_max_y = std::max(0.0, max_y + y_padding);
    const double origin_x = Canvas_X({0, 0}, plot_min_x, plot_max_x, width, margin);
    const double origin_y = Canvas_Y({0, 0}, plot_min_y, plot_max_y, height, margin);

    std::ofstream svg(path);
    if (!svg) {
        return;
    }

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width
        << "\" height=\"" << height << "\" viewBox=\"0 0 " << width << " " << height << "\">\n";
    svg << "<rect width=\"100%\" height=\"100%\" fill=\"#ffffff\"/>\n";
    svg << "<text x=\"30\" y=\"35\" font-family=\"Arial\" font-size=\"24\" fill=\"#111827\">Convex Hull - Upper Hull</text>\n";
    svg << "<line x1=\"" << margin << "\" y1=\"" << origin_y
        << "\" x2=\"" << width - margin << "\" y2=\"" << origin_y
        << "\" stroke=\"#9ca3af\" stroke-width=\"2\"/>\n";
    svg << "<line x1=\"" << origin_x << "\" y1=\"" << height - margin
        << "\" x2=\"" << origin_x << "\" y2=\"" << margin
        << "\" stroke=\"#9ca3af\" stroke-width=\"2\"/>\n";
    svg << "<text x=\"" << width - margin + 10 << "\" y=\"" << origin_y + 5
        << "\" font-family=\"Arial\" font-size=\"18\" fill=\"#6b7280\">x</text>\n";
    svg << "<text x=\"" << origin_x - 10 << "\" y=\"" << margin - 10
        << "\" font-family=\"Arial\" font-size=\"18\" fill=\"#6b7280\">y</text>\n";

    for (int x_value = min_x; x_value <= max_x; ++x_value) {
        const double x = Canvas_X({x_value, 0}, plot_min_x, plot_max_x, width, margin);
        svg << "<line x1=\"" << x << "\" y1=\"" << origin_y - 6
            << "\" x2=\"" << x << "\" y2=\"" << origin_y + 6
            << "\" stroke=\"#6b7280\" stroke-width=\"1.5\"/>\n";
        svg << "<text x=\"" << x - 6 << "\" y=\"" << origin_y + 24
            << "\" font-family=\"Arial\" font-size=\"14\" fill=\"#6b7280\">"
            << x_value << "</text>\n";
    }

    for (int y_value = min_y; y_value <= max_y; ++y_value) {
        const double y = Canvas_Y({0, y_value}, plot_min_y, plot_max_y, height, margin);
        svg << "<line x1=\"" << origin_x - 6 << "\" y1=\"" << y
            << "\" x2=\"" << origin_x + 6 << "\" y2=\"" << y
            << "\" stroke=\"#6b7280\" stroke-width=\"1.5\"/>\n";
        svg << "<text x=\"" << origin_x - 38 << "\" y=\"" << y + 5
            << "\" font-family=\"Arial\" font-size=\"14\" fill=\"#6b7280\">"
            << y_value << "</text>\n";
    }

    svg << "<polyline fill=\"none\" stroke=\"#2563eb\" stroke-width=\"3\" points=\"";
    for (int i = 1; i <= h; ++i) {
        svg << Canvas_X(UH[i], plot_min_x, plot_max_x, width, margin) << ","
            << Canvas_Y(UH[i], plot_min_y, plot_max_y, height, margin);
        if (i < h) {
            svg << " ";
        }
    }
    svg << "\"/>\n";

    for (int i = 1; i <= n; ++i) {
        const double x = Canvas_X(S[i], plot_min_x, plot_max_x, width, margin);
        const double y = Canvas_Y(S[i], plot_min_y, plot_max_y, height, margin);

        svg << "<circle cx=\"" << x << "\" cy=\"" << y
            << "\" r=\"6\" fill=\"#dc2626\"/>\n";
        svg << "<text x=\"" << x + 10 << "\" y=\"" << y - 10
            << "\" font-family=\"Arial\" font-size=\"16\" fill=\"#111827\">p"
            << i << " = (" << S[i].x << ", " << S[i].y << ")</text>\n";
    }

    svg << "</svg>\n";
}

#endif // CONVEX_HULL_UTILS_H
