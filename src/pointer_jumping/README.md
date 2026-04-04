# Pointer Jumping Notes

This folder contains a direct OpenMP-style translation of the pointer jumping pseudocode shown in class.

## Important note about synchronization

The PRAM pseudocode

```cpp
pardo (int i = 1; i <= n; ++i) {
    while (S[i] != S[S[i]]) {
        W[i] = W[i] + W[S[i]];
        S[i] = S[S[i]];
    }
}
```

assumes a **synchronous PRAM model**. In that model, all processors conceptually execute the same step at the same time. This means each "jump" is performed in a global round.

In OpenMP, however, the code above is **not automatically round-synchronous**:

- each thread executes its own `while` loop independently;
- different threads may perform a different number of iterations;
- one thread may read `S[j]` or `W[j]` while another thread is updating them.

Because of this, the code is **not a faithful implementation of the PRAM semantics**.

## Practical consequence

For plain pointer jumping (finding roots), the output may appear stable on the example input, but this is not a formal guarantee.

For pointer prefix sum on trees, the problem is easier to observe: the result can change across executions because `W[i]` may use a parent value `W[S[i]]` that has already been updated by another thread, or not yet updated, depending on scheduling.

This is why repeated executions may produce outputs such as:

- `W = [0, 4, 0, 2, -3, 7, 5, 0]`
- `W = [0, 4, 0, 2, -3, 9, 5, 0]`
- `W = [0, 4, 0, 2, -3, 5, 5, 0]`

## Why this cannot be fixed only with `pardo`

The macro `pardo` can parallelize the outer `for`, but it cannot make the inner `while` loop globally synchronous.

A correct PRAM-style implementation requires:

- global rounds;
- all threads reading the old state for a round;
- all threads writing the new state only after the round is complete.

In practice, this usually means using temporary arrays such as `nextS` and `nextW`, then swapping them at the end of each round.

## Summary

The current code is useful as a didactic translation of the pseudocode, but it should not be considered a fully correct OpenMP implementation of synchronous pointer jumping on a PRAM.
