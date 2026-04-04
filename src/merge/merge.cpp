#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "merge.hpp"
#include "parallel.hpp"

using namespace std;

static vector<int> Merge(const vector<int>& A, const vector<int>& B) {
    const int n = A.size() - 1;
    
    vector<int> RANK_A(n + 1, 0);
    vector<int> RANK_B(n + 1, 0);
    vector<int> C(2 * n + 1, 0);

    // PARTITIONING
    pardo (int i = 1; i <= n; ++i) {
        RANK_A[i] = compute_rank(A[i], B);
        RANK_B[i] = compute_rank(B[i], A);
    }

    // COMBINE
    pardo (int i = 1; i <= n; ++i) {
        C[i + RANK_A[i]] = A[i];
        C[i + RANK_B[i]] = B[i];
    }

    return C;
}

static vector<Pair> Generate_Subproblems(const vector<int>& A, const vector<int>& B, int s, int cLeftStart = 1) {
    const int n = A.size() - 1;
    const int m = B.size() - 1;
    const int k = (m + s - 1) / s;

    vector<int> J(k + 1, 0);
    vector<Pair> pairs;

    J[0] = 0;
    J[k] = n;

    pardo (int i = 1; i <= k - 1; ++i) {
        J[i] = compute_rank(B[i * s], A);
    }

    pairs.reserve(k);
    for (int i = 1; i <= k; ++i) {
        vector<int> A_i;
        vector<int> B_i;

        A_i.push_back(0);
        for (int j = J[i - 1] + 1; j <= J[i]; ++j) {
            A_i.push_back(A[j]);
        }

        B_i.push_back(0);
        for (int j = 1 + (i - 1) * s; j <= min(i * s, m); ++j) {
            B_i.push_back(B[j]);
        }

        pairs.push_back({A_i, B_i, cLeftStart + J[i - 1] + (i - 1) * s});
    }

    return pairs;
}

static void Sequential_Merge(const vector<int>& A, const vector<int>& B, int cLeft, vector<int>& C) {
    int i = 1;
    int j = 1;
    int k = cLeft;

    while (i < A.size() && j < B.size()) {
        if (A[i] <= B[j]) {
            C[k++] = A[i++];
        } else {
            C[k++] = B[j++];
        }
    }

    while (i < A.size()) {
        C[k++] = A[i++];
    }

    while (j < B.size()) {
        C[k++] = B[j++];
    }
}

vector<int> Optimal_Merge(const vector<int>& A, const vector<int>& B) {
    
    const int n = A.size() - 1;
    const int m = B.size() - 1;
    const int s = log2(n);

    vector<int> C(2 * n + 1, 0);

    assert(s > 0 && "Optimal_Merge assumes log2(n) is positive.");
    assert(n % s == 0 && "Optimal_Merge assumes n / log2(n) is integral.");

    /* DIVIDE */
    assert(m % s == 0 && "Generate_Subproblems assumes k = m / s is integral at the top level.");
    const vector<Pair> pairs = Generate_Subproblems(A, B, s);

    vector<vector<Pair>> local_pairs(n / s);

    pardo(int i = 1; i <= n / s; ++i) {
        const vector<int>& A_i = pairs[i - 1].A;
        const vector<int>& B_i = pairs[i - 1].B;
        const int C_i_left = pairs[i - 1].cLeft;

        if (A_i.size() - 1 > s) {
            const vector<Pair> refined_pairs = Generate_Subproblems(B_i, A_i, s, C_i_left);
            local_pairs[i - 1].reserve(refined_pairs.size());

            for (const Pair& pair : refined_pairs) {
                local_pairs[i - 1].push_back({pair.B, pair.A, pair.cLeft});
            }
        } else { /* it does nothing if |A_i| < log2(n) */
            local_pairs[i - 1].push_back({A_i, B_i, C_i_left});
        }
    }

    // These are the final pairs (A_1, B_1), ..., (A_h, B_h) of the pseudocode.
    vector<Pair> final_pairs;
    for (int i = 0; i < n / s; ++i) {
        final_pairs.insert(final_pairs.end(), local_pairs[i].begin(), local_pairs[i].end());
    }

    /* CONQUER AND COMBINE */
    const int h = final_pairs.size();

    pardo(int i = 0; i < h; ++i) {
        Sequential_Merge(
            final_pairs[i].A,
            final_pairs[i].B,
            final_pairs[i].cLeft,
            C
        );
    }

    return C;
}


int main() {
    vector<int> A = {1, 6, 7, 9};
    vector<int> B = {2, 3, 5, 8};
    vector<int> expected = {1, 2, 3, 5, 6, 7, 8, 9};

    assert(A.size() == B.size() && "This example assumes two arrays with the same number of elements.");

    push_front(A, 0);
    push_front(B, 0);
    push_front(expected, 0);

    assert(is_sorted(A.begin() + 1, A.end()) && "A must be sorted.");
    assert(is_sorted(B.begin() + 1, B.end()) && "B must be sorted.");
    assert(are_distinct(A, B) && "Merge assumes all elements are distinct.");

    
    cout << "A: ";
    printVector(A);
    cout << "B: ";
    printVector(B);

    vector<int> C = Merge(A, B);
    cout << "Merge:\n";
    cout << "C: ";
    printVector(C);

    assert(C == expected && "Merge returned an unexpected result.");


    C = Optimal_Merge(A, B);
    cout << "Optimal Merge:\n";
    cout << "C: ";
    printVector(C);

    assert(C == expected && "Optimal_Merge returned an unexpected result.");

    return 0;
}
