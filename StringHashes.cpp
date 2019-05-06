#include "StringHashes.h"
#include <iostream>
#include <cstdint>
#include <string>
using namespace std;

/**
 * From https://en.wikipedia.org/wiki/Jenkins_hash_function
 * @param input
 * @return value
 */
uint64_t JenkinsHash::hash(const std::string& input) const{
    size_t i = 0;
    uint32_t value = 0;
    int length = input.length();
    while (i != length) {
        value += input[i++];
        value += value << 10;
        value ^= value >> 6;
    }
    value += value << 3;
    value ^= value >> 11;
    value += value << 15;
    return value;
}

/**
 * From https://en.wikipedia.org/wiki/Jenkins_hash_function
 * @param input
 * @return
 */
uint64_t PearsonHash::hash(const std::string& input) const{
    uint64_t value = 0;
    for(uint8_t byte: input) {
        value = 255 - (value ^ byte);
    }

    return value;
}