
# Parallel Algorithms in C++

## Requirements

To compile and run the OpenMP-based algorithms in this repository, install the OpenMP library:

```bash
sudo apt-get update
sudo apt-get install libomp-dev
```

## Project Structure

- `src/` — C++ implementation of some parallel algorithms.
- `include/parallel.hpp` — Utility header: macro `pardo` and other utilities to facilitate parallel programming.
- `bin/` — Folder to store compiled executables.
- `src/prefix_sum/` — Recursive and iterative prefix sum implementations.
- `src/pointer_jumping/` — Pointer jumping algorithms on rooted forests.

## Compilation

To compile the programs:

```bash
g++ -fopenmp -Iinclude src/<folder_name>/<file_name>.cpp -o bin/<executable_name>
```

For example, to compile the prefix sum program:

```bash
g++ -fopenmp -Iinclude src/prefix_sum/prefix_sum.cpp -o bin/prefix_sum
```

To compile the pointer jumping program:

```bash
g++ -fopenmp -Iinclude src/pointer_jumping/pointer_jumping.cpp -o bin/pointer_jumping
```

## Macro `pardo` for Parallel Loops

The macro `pardo` allows you to write parallel for-loops in a style similar to PRAM pseudocode:

```cpp
#ifdef _OPENMP
  #define pardo _Pragma("omp parallel for") for
#else
  #define pardo for
#endif
```

Usage example:

```cpp
pardo (int i = 1; i <= n; ++i) {
    // parallel loop body
}
```

## Algorithms Provided

**Prefix Sum**
- Recursive Prefix Sum
- Iterative Prefix Sum

Both versions are implemented in `src/prefix_sum/prefix_sum.cpp`.

**Pointer Jumping**
- Root finding in a forest
- Pointer prefix sum on trees

Both versions are implemented in `src/pointer_jumping/pointer_jumping.cpp`.

Additional notes about the gap between PRAM pseudocode and OpenMP synchronization are available in `src/pointer_jumping/README.md`.

## Notes

- All arrays use 1-based indexing for full correspondence with theoretical pseudocode.
- The helper function `push_front` (in `include/parallel.hpp`) can be used to convert a 0-based vector to 1-based by inserting a dummy element at the front.
- The helper function `pow2(int exp)` is provided for readability when working with powers of two. I choose not to use `std::pow` for integer powers of two to avoid that the returned type is a double and needed a cast.
- The helper function `isPowerOfTwo(int n)` checks if the input size is valid for the algorithms.

---

For questions or suggestions, open an issue or contact the maintainer at [michaelpiccirilli3@gmail.com](mailto:michaelpiccirilli3@gmail.com).
