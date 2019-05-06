#include "IntegerHashes.h"
#include <iostream>
#include <cmath>
#include <cstdint>

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
    temp -= std::floor(temp);
    return temp * m;
}


/**
 * To initial the value of b
 */
SquareRootHash::SquareRootHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    this->b  = std::sqrt(4 * i + 3);
    this->b -= std::floor(this->b);
}

/**
 * This function also uses the "multiplication method" of hashing
 * It multiplies the input by the square root of 4i + 3, then multiplies the fractional part by m
 */
uint64_t SquareRootHash::hash(uint64_t input) const{
    double temp = input * b;
    temp -= std::floor(temp);
    return temp * m;
}
