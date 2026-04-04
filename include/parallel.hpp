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

inline int get_thread_id() {
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}

inline int get_max_threads() {
#ifdef _OPENMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}

inline void print_thread_iteration(int i) {
#ifdef _OPENMP
  #pragma omp critical
  {
    std::cout << "Thread ID: " << omp_get_thread_num() << " i: " << i << std::endl;
  }
#else
  std::cout << "Thread ID: 0 i: " << i << std::endl;
#endif
}

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

#endif // PARALLEL_UTILS_H
