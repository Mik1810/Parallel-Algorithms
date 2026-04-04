#ifndef PREFIX_SUM_UTILS_H
#define PREFIX_SUM_UTILS_H

inline int pow2(int exp) {
    return 1 << exp;
}

inline bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

#endif // PREFIX_SUM_UTILS_H
