#ifndef POINTER_JUMPING_UTILS_H
#define POINTER_JUMPING_UTILS_H

inline bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

#endif // POINTER_JUMPING_UTILS_H
