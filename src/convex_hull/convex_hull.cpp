#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "convex_hull.hpp"
#include "parallel.hpp"

using namespace std;

static vector<Point> Sequential_Upper_Hull(const vector<Point>& S) {
    const int n = S.size() - 1;

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
    const int a = Argmax_Y(UH1);
    const int b = Argmax_Y(UH2);

    if (UH1[a].y < UH2[b].y) {
        const int z2 = b;
        const int z1 = Find_Min_Slope_Point(UH1, a, UH2[z2]);
        assert(is_upper_tangent(UH1, UH2, z1, z2) && "The tangent endpoint on UH1 is inconsistent.");
        return {z1, z2};
    }

    const int z1 = a;
    const int z2 = Find_Max_Slope_Point(UH1[z1], UH2, b);
    assert(is_upper_tangent(UH1, UH2, z1, z2) && "The tangent endpoint on UH2 is inconsistent.");
    return {z1, z2};
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
        {1, 1},
        {2, 4},
        {3, 10},
        {4, 8},
        {5, 14},
        {6, 9},
        {7, 7},
        {8, 2}
    };

    vector<Point> expected = {
        {1, 1},
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

    Write_SVG(S, UH, "src/convex_hull/convex_hull.svg");
    cout << "SVG: convex_hull.svg\n";

    return 0;
}
