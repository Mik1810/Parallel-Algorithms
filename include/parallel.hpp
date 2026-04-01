#ifndef PARALLEL_UTILS_H
#define PARALLEL_UTILS_H

#include <iostream>
#include <vector>

#ifdef _OPENMP
  #include <omp.h>
  #define pardo _Pragma("omp parallel for") for
#else
  #define pardo for
#endif

// Funzione inline per evitare multiple definition
inline void printVector(const std::vector<int>& vec) {
    int n = vec.size() - 1;
    std::cout << "[";
    for (int i = 1; i <= n; ++i) {
        std::cout << vec[i];
        if (i < n) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Utility: insert an element at the front of a vector<int> (1-based convenience)
inline void push_front(std::vector<int>& A, int value) {
  A.insert(A.begin(), value);
}

inline int pow2(int exp) {
    return 1 << exp; // 2^exp using bit shift
}

inline bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}


#endif // PARALLEL_UTILS_H