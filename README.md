
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
- `src/merge/` — Merge algorithms from the Divide and Conquer lesson.
- `src/convex_hull/` — Upper hull prototype from the convex hull lesson.

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

To compile the merge program:

```bash
g++ -fopenmp -Iinclude src/merge/merge.cpp -o bin/merge
```

To compile the convex hull program:

```bash
g++ -fopenmp -Iinclude src/convex_hull/convex_hull.cpp -o bin/convex_hull
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

**Divide and Conquer**
- Parallel merge by ranks and binary search
- Work-optimal merge via subproblem generation

Both versions are implemented in `src/merge/merge.cpp`.

- Upper hull by divide and conquer

The current convex hull prototype is implemented in `src/convex_hull/convex_hull.cpp`.

## Notes

- All arrays use 1-based indexing for full correspondence with theoretical pseudocode.
- The helper function `push_front` (in `include/parallel.hpp`) can be used to convert a 0-based vector to 1-based by inserting a dummy element at the front.
- The helper functions `pow2(int exp)` and `isPowerOfTwo(int n)` are defined in `src/prefix_sum/prefix_sum.hpp`, so the prefix-sum-specific utilities stay local to that lesson.
- The convex hull lesson uses its own local utilities in `src/convex_hull/convex_hull.hpp`.

---

For questions or suggestions, open an issue or contact the maintainer at [michaelpiccirilli3@gmail.com](mailto:michaelpiccirilli3@gmail.com).
