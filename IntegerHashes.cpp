#include "IntegerHashes.h"
#include <cmath>

IntegerHash::IntegerHash(uint64_t i, uint64_t m) {
    this->i = i;
    this->m = m;
}

DivisionHash::DivisionHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    // Nothing else to do.
}

uint64_t DivisionHash::hash(uint64_t input) const {
    return input * (i + 1) % m;
}

ReciprocalHash::ReciprocalHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    this->b = double(1) / (i + 2);
}

uint64_t ReciprocalHash::hash(uint64_t input) const {
    double temp = b * input;
    temp -= std::floor(temp);
    return temp * m;
}

SquareRootHash::SquareRootHash(uint64_t i, uint64_t m): IntegerHash(i, m) {
    this->b  = std::sqrt(4 * i + 3);
    this->b -= std::floor(this->b);
}

uint64_t SquareRootHash::hash(uint64_t input) const {
    double temp = b * input;
    temp -= std::floor(temp);
    return temp * m;
}
