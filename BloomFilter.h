#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "IntegerHashes.h"
#include "StringHashes.h"
#include <cstdint>
#include <string>

class BloomFilter {
  int k; // The number of hash functions
  int m; // The number or bits

  StringHash*   strfn;  // The function used to hash strings to integers
  IntegerHash** intfns; // The functions used to map those integers to bit indices
  uint64_t*     bits;   // The bits themselves

public:
  BloomFilter(int k, int m, std::string intfn, std::string strfn);
  ~BloomFilter();

  void insert(const std::string& value);
  bool lookup(const std::string& value) const;
};

#endif
