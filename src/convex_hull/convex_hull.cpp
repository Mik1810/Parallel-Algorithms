#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "convex_hull.hpp"
#include "parallel.hpp"

using namespace std;

static vector<Point> Sequential_Upper_Hull(const vector<Point>& S) {
    const int n = int(S.size()) - 1;

    if (n <= 2) {
        return S;
    }

    vector<Point> UH(1);
    for (int i = 1; i <= n; ++i) {
        while (UH.size() >= 3 &&
               orientation(UH[UH.size() - 2], UH[UH.size() - 1], S[i]) >= 0) {
            UH.pop_back();
        }
        UH.push_back(S[i]);
    }

    return UH;
}

static pair<int, int> Upper_Common_Tangent(const vector<Point>& UH1, const vector<Point>& UH2) {
    const int n1 = int(UH1.size()) - 1;
    const int n2 = int(UH2.size()) - 1;

    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            if (is_upper_tangent(UH1, UH2, i, j)) {
                return {i, j};
            }
        }
    }

    assert(false && "Upper common tangent not found.");
    return {0, 0};
}

static vector<Point> Combine_Upper_Hulls(const vector<Point>& UH1, const vector<Point>& UH2) {
    const auto [i, j] = Upper_Common_Tangent(UH1, UH2);
    const int n2 = int(UH2.size()) - 1;

    vector<Point> UH(1);
    for (int t = 1; t <= i; ++t) {
        UH.push_back(UH1[t]);
    }

    for (int t = j; t <= n2; ++t) {
        UH.push_back(UH2[t]);
    }

    return UH;
}

static vector<Point> Upper_Hull(const vector<Point>& S) {
    const int n = S.size() - 1;

    if (n <= 4) {
        return Sequential_Upper_Hull(S);
    }

    vector<Point> S1(n / 2 + 1);
    vector<Point> S2(n / 2 + 1);

    pardo (int i = 1; i <= n; ++i) {
        if (i <= n / 2) {
            S1[i] = S[i];
        } else {
            S2[i - n / 2] = S[i];
        }
    }

    vector<Point> UH1(1);
    vector<Point> UH2(1);

    pardo (int i = 1; i <= 2; ++i) {
        if (i == 1) {
            UH1 = Upper_Hull(S1);
        } else {
            UH2 = Upper_Hull(S2);
        }
    }

    return Combine_Upper_Hulls(UH1, UH2);
}

int main() {
    vector<Point> S = {
        {1, 0},
        {2, 4},
        {3, 10},
        {4, 8},
        {5, 14},
        {6, 9},
        {7, 7},
        {8, 2}
    };

    vector<Point> expected = {
        {1, 0},
        {3, 10},
        {5, 14},
        {7, 7},
        {8, 2}
    };

    push_front(S, {0, 0});
    push_front(expected, {0, 0});

    assert(is_sorted_by_x(S) && "S must be sorted by x-coordinate.");
    assert(have_distinct_x_and_y(S) && "The lecture assumes all x and all y coordinates are distinct.");
    assert(has_no_three_collinear_points(S) && "The lecture assumes that S contains no three collinear points.");

    const vector<Point> UH = Upper_Hull(S);

    cout << "S: ";
    printPoints(S);
    cout << "Upper Hull:\n";
    cout << "UH(S): ";
    printPoints(UH);

    assert(UH == expected && "Upper_Hull returned an unexpected result.");

    return 0;
}
