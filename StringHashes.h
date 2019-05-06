#ifndef STRINGHASHES_H
#define STRINGHASHES_H

#include <cstdint>
#include <string>

// The functions in this file take strings and map them to integers.
// To get integer values suitable for use in a Bloom filter or hash
// table, take the outputs of these functions and run them through
// the integer hash functions declared in IntegerHashes.h.

class StringHash {
public:
  virtual ~StringHash() {}
  virtual uint64_t hash(const std::string& input) const = 0;
};

class JenkinsHash: public StringHash {
  // This implements Bob Jenkins' "one at a time" hash function
public:
  uint64_t hash(const std::string& input) const;
};

class PearsonHash: public StringHash {
  // This implements Peter Pearson's eight-bit hash
  // It uses a very simple lookup table: T[i] = 255 - i
public:
  uint64_t hash(const std::string& input) const;
};

#endif
