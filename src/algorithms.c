#include "algorithms.h"
#include "characters_to_base.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BLOCK_SIZE 64  // Block size for cache-aware
#define BASE_CASE_SIZE 32  // Base case size for cache-oblivious

int min3(int a, int b, int c) {
    int min = (a < b) ? a : b;
    return (min < c) ? min : c;
}

// Calculates the cost of aligning two characters considering genetic base rules
static inline int get_alignment_cost(char x, char y) {
    // If any character is not a base, we ignore it
    if (!isBase(x) || !isBase(y)) return 0;
    
    // Convert to uppercase for comparison
    x = CharToBase(x);
    y = CharToBase(y);
    
    // If they are equal, the cost is 0
    if (x == y) return 0;
    
    // If either is N (unknown base) or they are different, the cost is 1
    return (isUnknownBase(x) || isUnknownBase(y) || x != y) ? 1 : 0;
}

// Iterative implementation with space O(N+M)
int iterative_alignment(const char* X, size_t M, const char* Y, size_t N) {
    // Allocate two rows for dynamic programming
    int* current = (int*)malloc((N + 1) * sizeof(int));
    int* previous = (int*)malloc((N + 1) * sizeof(int));
    
    if (!current || !previous) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    // Initialize the first row
    for (size_t j = 0; j <= N; j++) {
        previous[j] = j * 2;  // Insertion cost = 2
    }

    // Calculate row by row
    for (size_t i = 1; i <= M; i++) {
        current[0] = i * 2;  // Insertion cost = 2
        
        for (size_t j = 1; j <= N; j++) {
            // Calculate the three possible cases
            int match_cost = get_alignment_cost(X[i-1], Y[j-1]);
            int case1 = previous[j-1] + match_cost;
            int case2 = previous[j] + 2;     // Insertion
            int case3 = current[j-1] + 2;    // Deletion
            
            current[j] = min3(case1, case2, case3);
        }
        
        // Swap the rows
        int* temp = previous;
        previous = current;
        current = temp;
    }

    // The result is in the last cell of previous
    int result = previous[N];
    
    free(current);
    free(previous);
    
    return result;
}

int cache_aware_alignment(const char* X, size_t M, const char* Y, size_t N, 
                         size_t Z, size_t L) {
    // Calculate the optimal block size based on Z and L
    size_t block_size = (Z / (3 * L)) * L;
    if (block_size == 0) block_size = L;

    // Complete matrix for dynamic programming
    int* dp = (int*)malloc((M + 1) * (N + 1) * sizeof(int));
    if (!dp) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    // Initialize the first row and column
    for (size_t i = 0; i <= M; i++) {
        dp[i * (N + 1)] = i * 2;
    }
    for (size_t j = 0; j <= N; j++) {
        dp[j] = j * 2;
    }

    // Process by blocks
    for (size_t i_block = 1; i_block <= M; i_block += block_size) {
        size_t i_end = (i_block + block_size < M) ? i_block + block_size : M;
        
        for (size_t j_block = 1; j_block <= N; j_block += block_size) {
            size_t j_end = (j_block + block_size < N) ? j_block + block_size : N;
            
            // Process the current block
            for (size_t i = i_block; i <= i_end; i++) {
                for (size_t j = j_block; j <= j_end; j++) {
                    int match_cost = get_alignment_cost(X[i-1], Y[j-1]);
                    int case1 = dp[(i-1) * (N + 1) + (j-1)] + match_cost;
                    int case2 = dp[(i-1) * (N + 1) + j] + 2;
                    int case3 = dp[i * (N + 1) + (j-1)] + 2;
                    
                    dp[i * (N + 1) + j] = min3(case1, case2, case3);
                }
            }
        }
    }

    int result = dp[M * (N + 1) + N];
    free(dp);
    return result;
}

// Helper function for the cache-oblivious algorithm
static void base_case_alignment(const char* X, size_t x_start, size_t x_end,
                              const char* Y, size_t y_start, size_t y_end,
                              int* buffer1, int* buffer2) {
    size_t m = x_end - x_start;
    size_t n = y_end - y_start;
    
    // Initialize buffer1
    for (size_t j = 0; j <= n; j++) {
        buffer1[j] = j * 2;
    }
    
    // Calculate the alignment
    for (size_t i = 1; i <= m; i++) {
        buffer2[0] = i * 2;
        
        for (size_t j = 1; j <= n; j++) {
            int match_cost = get_alignment_cost(X[x_start + i - 1], Y[y_start + j - 1]);
            int case1 = buffer1[j-1] + match_cost;
            int case2 = buffer1[j] + 2;
            int case3 = buffer2[j-1] + 2;
            
            buffer2[j] = min3(case1, case2, case3);
        }
        
        // Swap the buffers
        int* temp = buffer1;
        buffer1 = buffer2;
        buffer2 = temp;
    }
}

static int cache_oblivious_recursive(const char* X, size_t x_start, size_t x_end,
                                   const char* Y, size_t y_start, size_t y_end,
                                   int* buffer1, int* buffer2) {
    size_t m = x_end - x_start;
    size_t n = y_end - y_start;
    
    // Base case
    if (m <= BASE_CASE_SIZE || n <= BASE_CASE_SIZE) {
        base_case_alignment(X, x_start, x_end, Y, y_start, y_end, buffer1, buffer2);
        return buffer1[n];
    }
    
    // Divide the problem into two
    size_t mid;
    if (m > n) {
        mid = x_start + m/2;
        // Recursively solve the two halves
        cache_oblivious_recursive(X, x_start, mid, Y, y_start, y_end, buffer1, buffer2);
        cache_oblivious_recursive(X, mid, x_end, Y, y_start, y_end, buffer1, buffer2);
    } else {
        mid = y_start + n/2;
        // Recursively solve the two halves
        cache_oblivious_recursive(X, x_start, x_end, Y, y_start, mid, buffer1, buffer2);
        cache_oblivious_recursive(X, x_start, x_end, Y, mid, y_end, buffer1, buffer2);
    }
    
    return buffer1[n];
}

int cache_oblivious_alignment(const char* X, size_t M, const char* Y, size_t N) {
    // Allocate the necessary buffers
    int* buffer1 = (int*)malloc((N + 1) * sizeof(int));
    int* buffer2 = (int*)malloc((N + 1) * sizeof(int));
    
    if (!buffer1 || !buffer2) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    int result = cache_oblivious_recursive(X, 0, M, Y, 0, N, buffer1, buffer2);
    
    free(buffer1);
    free(buffer2);
    
    return result;
}