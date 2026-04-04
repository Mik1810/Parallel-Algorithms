#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "./merge.hpp"
#include "parallel.hpp"

using namespace std;

static vector<int> Merge(const vector<int>& A, const vector<int>& B) {
    const int n = static_cast<int>(A.size()) - 1;
    
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

vector<int> Optimal_Merge(const vector<int>& A, const vector<int>& B) {
    vector<int> C(A.size() + B.size() - 2, 0); // -2 because A[0] and B[0] are dummy

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

    const vector<int> mergeByRanks = Merge(A, B);
    cout << "Merge:\n";
    cout << "C: ";
    printVector(mergeByRanks);

    cout << "Optimal Merge:\n";
    cout << "Not implemented yet.\n";

    assert(mergeByRanks == expected && "Merge returned an unexpected result.");

    return 0;
}
