#include "BloomFilter.h"
#include <iostream>
#include <cstdint>
using namespace std;

/**
 *
 * @param k The number of hash functions
 * @param m The number or bits
 * @param intfn The functions used to map those integers to bit indices
 * @param strfn The function used to hash strings to integers
 */
BloomFilter::BloomFilter(int k, int m, string strfn, string intfn){
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
            this->intfns[i] = new DivisionHash(i, m);
        }
        else if (intfn == "reciprocal"){
            this->intfns[i] = new ReciprocalHash(i, m);
        }
        else if (intfn == "squareroot")
            this->intfns[i] = new SquareRootHash(i, m);
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

void BloomFilter::insert(const string& value){
    uint64_t key = strfn->hash(value);
    for (int i = 0; i < k; i++) {
        uint64_t hashValue = intfns[i]->hash(key);
        bits[hashValue] = 1;
    }
}

bool BloomFilter::lookup(const string& value) const{
    uint64_t key = strfn->hash(value);
    for (int i = 0; i < k; i++) {
        uint64_t hashValue = intfns[i]->hash(key);
        // The default value of bits[i] is 0
        if (bits[hashValue] == 0) {
            return false;
        }
    }
    return true;
}
