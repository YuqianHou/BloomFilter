#include "IntegerHashes.h"
#include <iostream>
#include <cmath>
#include <cstdint>
using namespace std;

// The functions in this file hash arbitrary 64-bit integers.
// Unlike the functions in StringHashes.h, these functions are aware
// of the size of the hash table or Bloom filter they are used with,
// and reduce their output to the correct range.

// The variable m is the size of the table.  The variable i is the
// index of the hash function within a Bloom filter, which makes sure
// the hash functions behave differently.  A Bloom filter with k hash
// functions will have functions with indices 0, 1, 2, ..., k-1.

IntegerHash::IntegerHash(uint64_t i, uint64_t m) {
    this->i = i;
    this->m = m;
}

DivisionHash::DivisionHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    // Nothing else to do
}

/**
 * This function uses the "division method" of hashing
 * It first multiplies the input by i + 1, then reduces the result modulo m
 */
uint64_t DivisionHash::hash(uint64_t input) const{
    return input * (i + 1) % m;
}


/**
 * To initial the value of b
 */
ReciprocalHash::ReciprocalHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    this->b = double(1) / (i + 2);
}

/**
 * This function uses the "multiplication method" of hashing
 * It multiplies the input by 1 / (i + 2), then multiplies the fractional part by m
 */
uint64_t ReciprocalHash::hash(uint64_t input) const{
    double temp = input * b;
    temp -= floor(temp);
    return temp * m;
}


/**
 * To initial the value of b
 */
SquareRootHash::SquareRootHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    this->b  = sqrt(4 * i + 3);
    this->b -= floor(this->b);
}

/**
 * This function also uses the "multiplication method" of hashing
 * It multiplies the input by the square root of 4i + 3, then multiplies the fractional part by m
 */
uint64_t SquareRootHash::hash(uint64_t input) const{
    double temp = input * b;
    temp -= floor(temp);
    return temp * m;
}
