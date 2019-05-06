#ifndef INTEGERHASHES_H
#define INTEGERHASHES_H

#include <cstdint>

// The functions in this file hash arbitrary 64-bit integers.
// Unlike the functions in StringHashes.h, these functions are aware
// of the size of the hash table or Bloom filter they are used with,
// and reduce their output to the correct range.

// The variable m is the size of the table.  The variable i is the
// index of the hash function within a Bloom filter, which makes sure
// the hash functions behave differently.  A Bloom filter with k hash
// functions will have functions with indices 0, 1, 2, ..., k-1.

class IntegerHash {
protected:
  uint64_t i;
  uint64_t m;
public:
  IntegerHash(uint64_t i, uint64_t m);
  virtual ~IntegerHash() {}

  virtual uint64_t hash(uint64_t input) const = 0;
};

class DivisionHash: public IntegerHash {
  // This function uses the "division method" of hashing
  // It first multiplies the input by i + 1, then reduces the result modulo m
public:
  DivisionHash(uint64_t i, uint64_t m);
  uint64_t hash(uint64_t input) const;
};

class ReciprocalHash: public IntegerHash {
  // This function uses the "multiplication method" of hashing
  // It multiplies the input by 1 / (i + 2), then multiplies the fractional part by m
  double b;
public:
  ReciprocalHash(uint64_t i, uint64_t m);
  uint64_t hash(uint64_t input) const;
};

class SquareRootHash: public IntegerHash {
  // This function also uses the "multiplication method" of hashing
  // It multiplies the input by the square root of 4i + 3, then multiplies the fractional part by m
  double b;
public:
  SquareRootHash(uint64_t i, uint64_t m);
  uint64_t hash(uint64_t input) const;
};

#endif
