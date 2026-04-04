#include <atomic>
#include <iostream>
#include <vector>
#include <cassert>

#include "parallel.hpp"

using namespace std;

static vector<int> Pointer_Jumping(const int n, const vector<int>& P) {
    vector<int> R(n + 1, 0);

    // To avoid race condition of the threads, first copy P to R
    // then perform pointer jumping on R 
    pardo (int i = 1; i <= n; ++i) R[i] = P[i];
    
    pardo (int i = 1; i <= n; ++i) {
        while(R[i] != R[R[i]]) {
            R[i] = R[R[i]];
        }
    }

    return R;
}

static vector<int> Pointer_Prefix_Sum_On_Trees(const int n, const vector<int>& P, vector<int>& W) {
    
    vector<int> S(n + 1, 0);

    // To avoid race condition of the threads, first copy P to S
    // then perform pointer jumping on S and W
    pardo (int i = 1; i <= n; ++i) S[i] = P[i];

    pardo (int i = 1; i <= n; ++i) {
        while (S[i] != S[S[i]]) {
            W[i] = W[i] + W[S[i]];
            S[i] = S[S[i]];
        }
    }

    return W;
}

int main() {
    // Slide example (n = 8):
    // index: 1 2 3 4 5 6 7 8
    //   P :  5 3 3 8 3 7 4 8
    vector<int> P = {5, 3, 3, 8, 3, 7, 4, 8}; // 0-based input
    int n = P.size();
    assert((n & (n-1)) == 0 && "n must be a power of 2!");
    push_front(P, 0); // make it 1-based (P[0] unused)

    cout << "P: ";
    printVector(P);

    const vector<int> R = Pointer_Jumping(n, P);
    cout << "Pointer Jumping Result:\n";
    cout << "R: ";
    // Expected output:
    // index: 1 2 3 4 5 6 7 8
    //   R :  3 3 3 8 3 8 8 8
    printVector(R);

    vector<int> W = {3, 4, 0, 2, -3, 2, 3, 0};
    assert(W.size() == n && "W must have the same size as P!"); 
    push_front(W, 0); // make it 1-based (W[0] unused)

    W = Pointer_Prefix_Sum_On_Trees(n, P, W);
    cout << "Pointer Prefix Sum on Trees Result:\n";
    // Expected output:
    // index: 1 2 3 4 5 6 7 8
    //   W :  0 4 0 2 -3 7 5 0
    cout << "W: ";
    printVector(W);

    return 0;
}
