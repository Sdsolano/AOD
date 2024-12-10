#ifndef CHARACTERS_TO_BASE_H
#define CHARACTERS_TO_BASE_H

#include <ctype.h>

// Checks if a character is an unknown base (N or n)
static inline int isUnknownBase(char c) {
    return (c == 'N' || c == 'n');
}

// Checks if a character is a valid base (A, C, G, T, U, N in uppercase or lowercase)
static inline int isBase(char c) {
    c = toupper(c);
    return (c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'N');
}

// Converts a character to its corresponding base (in uppercase)
static inline char CharToBase(char c) {
    return isBase(c) ? toupper(c) : '\0';
}

#endif // CHARACTERS_TO_BASE_H