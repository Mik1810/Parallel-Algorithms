#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include "parallel.hpp"
#include "prefix_sum.hpp"

using namespace std;

// 1-based algorithm for prefix sum
vector<int> Prefix_Sum_recursive(int n, const std::vector<int>& A) {
    
    // Base case: single element
    vector<int> B(n+1, 0); // 1-based, B[0] unused
    if (n == 1) { B[1] = A[1]; return B; }

    // Step 2: Compute X[i] = A[2i-1] + A[2i] for 1 <= i <= n/2
    vector<int> X(n/2 + 1, 0);

    pardo (int i = 1; i <= n/2; ++i) {
        X[i] = A[2*i - 1] + A[2*i];
    }

    // Step 3: Y = Prefix_Sum(n/2, X)
    vector<int> Y = Prefix_Sum_recursive(n/2, X);

    // Step 4: Z[i] = Y[i]
    vector<int> Z(n/2 + 1, 0);
    pardo (int i = 1; i <= n/2; ++i) {
        Z[i] = Y[i];
    }

    // Step 5-8: Compute B
    pardo (int i = 1; i <= n; ++i) {
        if (i == 1) {
            B[1] = A[1];
        } else if (i % 2 == 0) { // i even
            B[i] = Y[i/2];
        } else { // i odd
            B[i] = Z[(i-1)/2] + A[i];
        }
    }
    return B;
}

vector<int> Prefix_Sum_iterative(int n, const std::vector<int>& A) {
    vector<vector<int>> B(log2(n) + 1, vector<int>(n + 1, 0));

    pardo (int i = 1; i <= n; ++i) {
        B[0][i] = A[i];
    }

    // FORWARD TRAVERSAL
    for (int j = 1; j <= log2(n); ++j) {
        pardo (int i = 1; i <= n / pow2(j); ++i) {
            B[j][i] = B[j - 1][2 * i - 1] + B[j - 1][2 * i];
        }
    }

    // BACKWARD TRAVERSAL
    for (int j = log2(n) - 1; j >= 0; --j) {
        pardo (int i = 1; i <= n / pow2(j); ++i) {
            if (i % 2 == 0) {
                B[j][i] = B[j + 1][i / 2];
            } else if(i % 2 != 0 && i > 1) {
                B[j][i] = B[j + 1][(i - 1) / 2] + B[j][i];
            }
        }
    }

    return B[0];
}


int main() {
    
    vector<int> A = {3, 4, -1, 5, -3, 2, 3, -8};
    vector<int> expected = {3, 7, 6, 11, 8, 10, 13, 5};
    int n = A.size();
    assert(isPowerOfTwo(n) && "n must be a power of 2!");
    push_front(A, 0); // Add dummy element at index 0 for 1-based indexing
    push_front(expected, 0);

    cout << "OpenMP max threads available: " << get_max_threads() << std::endl;

    cout << "A: ";
    printVector(A);
    vector<int> B;

    B = Prefix_Sum_recursive(n, A);

    cout << "Recursive:\n";
    cout << "B: ";
    printVector(B);
    assert(B == expected && "Prefix_Sum_recursive returned an unexpected result.");

    B = Prefix_Sum_iterative(n, A);

    cout << "Iterative:\n";
    cout << "B: ";
    printVector(B);
    assert(B == expected && "Prefix_Sum_iterative returned an unexpected result.");
    return 0;
}
