#ifndef MERGE_UTILS_H
#define MERGE_UTILS_H

#include <algorithm>
#include <vector>

struct Pair {
    std::vector<int> A;
    std::vector<int> B;
    int cLeft;
};

inline bool are_distinct(const std::vector<int>& A, const std::vector<int>& B) {
    const int n = static_cast<int>(A.size()) - 1;
    const int m = static_cast<int>(B.size()) - 1;

    int i = 1;
    int j = 1;
    while (i <= n && j <= m) {
        if (A[i] == B[j]) return false;
        if (A[i] < B[j]) {
            ++i;
        } else {
            ++j;
        }
    }

    return true;
}

inline int compute_rank_in_range(const std::vector<int>& Y, int left, int right, int x) {
    if (left > right) return 0;

    const auto first = Y.begin() + left;
    const auto last = Y.begin() + right + 1;
    return static_cast<int>(std::lower_bound(first, last, x) - first);
}

inline int compute_rank(int x, const std::vector<int>& Y) {
    return compute_rank_in_range(Y, 1, static_cast<int>(Y.size()) - 1, x);
}

#endif
