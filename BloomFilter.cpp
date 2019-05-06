#include "BloomFilter.h"
#include <iostream>
#include <cstdint>
#include <string>

/**
 *
 * @param k The number of hash functions
 * @param m The number or bits
 * @param intfn The functions used to map those integers to bit indices
 * @param strfn The function used to hash strings to integers
 */
BloomFilter::BloomFilter(int k, int m, std::string intfn, std::string strfn){
    this->k = k;
    this->m = m;
    this->bits = new uint64_t[m];
    for (int i = 0; i < m; i++) {
        bits[i] = 0;
    }
    if (strfn == "jenkins") {
        this->strfn = new JenkinsHash();
    }
    else if(strfn == "pearson")
        this->strfn = new PearsonHash();
    
    intfns = new IntegerHash*[k];
    for (int i = 0; i < k; i++) {
        if (intfn == "division") {
            this->intfns[i] = new DivisionHash((uint64_t)i, (uint64_t)m);
        }
        else if (intfn == "reciprocal"){
            this->intfns[i] = new ReciprocalHash((uint64_t)i, (uint64_t)m);
        }
        else if (intfn == "squareroot")
            this->intfns[i] = new SquareRootHash((uint64_t)i, (uint64_t)m);
    }
}

/**
 * Destructor
 */
BloomFilter::~BloomFilter(){
    delete [] bits;
    delete strfn;
    for (int i = 0; i < k; i++) {
        delete intfns[i];
    }
    delete [] intfns;
}

void BloomFilter::insert(const std::string& value){
    uint64_t key = strfn->hash(value);
    
    for (int i = 0; i < k; i++) {
        uint64_t hashValue = intfns[i]->hash(key);
        bits[hashValue] = (uint64_t)1;
    }
}

bool BloomFilter::lookup(const std::string& value) const{
    uint64_t key = strfn->hash(value);
    for (int i = 0; i < k; i++) {
        uint64_t hashValue = intfns[i]->hash(key);
        // The default value of bits[i] is 0
        if (bits[hashValue] == (uint64_t)0) {
            return false;
        }
    }
    return true;
}
