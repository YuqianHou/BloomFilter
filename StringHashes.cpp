#include "StringHashes.h"
#include <iostream>
#include <cstdint>

// From https://en.wikipedia.org/wiki/Jenkins_hash_function
uint64_t JenkinsHash::hash(const std::string& input) const{
    uint32_t hash = 0;
    for(uint8_t byte: input) {
        hash += byte;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

// From https://en.wikipedia.org/wiki/Jenkins_hash_function
uint64_t PearsonHash::hash(const std::string& input) const{
    uint64_t hash = 0;
    for(uint8_t byte: input) {
        hash = 255 - (hash ^ byte);
    }
    
    return hash;
}
