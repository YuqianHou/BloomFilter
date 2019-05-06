#include "BloomFilter.h"
#include <iostream>
using namespace std;

/**
 *
 * @param k The number of hash functions
 * @param m The number or bits
 * @param intfn The functions used to map those integers to bit indices
 * @param strfn The function used to hash strings to integers
 */
BloomFilter::BloomFilter(int k, int m, std::string intfn, std::string strfn){
    this->k = k;    // The number of hash functions
    this->m = m;    // The number or bits
    if (strfn == "jenkins") {
        this->strfn = new JenkinsHash();
    }
    else
        this->strfn = new PearsonHash();
    for (int i = 0; i < k; i++) {
        if (intfn == "division") {
            this->intfns[i] = new DivisionHash(i, m);
        }
        else if (intfn == "reciprocal"){
            this->intfns[i] = new ReciprocalHash(i, m);
        }
        else
            this->intfns[i] = new SquareRootHash(i, m);
    }
}

/**
 * Destructor
 */
BloomFilter::~BloomFilter(){
    delete intfns;
    delete strfn;
}

/**
 *
 * @param value
 */
void BloomFilter::insert(const std::string& value){
    uint64_t hashValue = strfn->hash(value);
    for (int i = 0; i < k; i++) {
        hashValue = intfns[i]->hash(hashValue);
        bits[hashValue] = 1;
    }
}

/**
 * @param value
 * @return
 */
bool BloomFilter::lookup(const std::string& value) const{
    uint64_t hashValue = strfn->hash(value);
    for (int i = 0; i < k; i++) {
        hashValue = intfns[i]->hash(hashValue);
        // The default value of bits[i] is 0
        if (bits[hashValue] == 1) {
            return true;
        }
    }
    return false;
}