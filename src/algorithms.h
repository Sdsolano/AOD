#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stddef.h>

// Auxiliary function to find the minimum of three values
int min3(int a, int b, int c);

// Iterative algorithm (bottom-up) with space complexity O(N)
int iterative_alignment(const char* X, size_t M, const char* Y, size_t N);

// Cache-aware algorithm with cache parameters
int cache_aware_alignment(const char* X, size_t M, const char* Y, size_t N, 
                         size_t Z, size_t L);

// Cache-oblivious algorithm
int cache_oblivious_alignment(const char* X, size_t M, const char* Y, size_t N);

#endif // ALGORITHMS_H