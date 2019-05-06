#include "HashSet.h"
#include <iostream>
#include <cstdint>
#include <string>
using namespace std;

/*
int           nitems; // The number of items in the set
int           nslots; // The number of slots in the table
IntegerHash*  intfn;  // The integer hash function to use
StringHash*   strfn;  // The string hash function to use
StringHash*   strfn2; // The hash function to use for probing (if you do double hashing)
std::string** slots;  // The slots themselves
*/
void HashSet::rehash(){
    string temp[nitems];
    for (int i = 0, j = 0; j < nslots; j++) {
        if(slots[j] != NULL){
            temp[i] = *slots[j];
            i++;
        }
    }
    for (int i = 0; i < nslots; i++) {
        delete slots[i];
        slots[i] = NULL;
    }
    nslots *= 2;
    slots = new string*[nslots];
    for (int i = 0; i < nslots; i++) {
        slots[i] = NULL;
    }
    for (int i = 0; i < nitems; i++) {
        uint64_t hashValue = strfn->hash(temp[i]);
        hashValue = intfn->hash(hashValue);
        for (int j = 0; ; j++) {
            if (slots[(hashValue + j) % nslots] == NULL) {
                slots[(hashValue + j) % nslots] = new string(temp[i]);
                break;
            }
        }
    }
}

HashSet::HashSet(){
    this->nitems = 0;
    this->nslots = 100;
    this->slots = new string*[nslots];
    this->intfn = new DivisionHash(0, nslots);
    this->strfn = new JenkinsHash();
    for (int i = 0; i < nslots; i++) {
        slots[i] = NULL;
    }
}

HashSet::~HashSet(){
    delete intfn;
    delete strfn;
    // delete slots;
    for (int i = 0; i < nslots; i++) {
        delete slots[i];
    }
}

void HashSet::insert(const std::string& value){
    uint64_t hashValue = strfn->hash(value);
    hashValue = intfn->hash(hashValue);
    for (int i = 0; ; i++) {
        if (slots[(hashValue + i) % nslots] == NULL) {
            //*slots[(hashValue + i) % nslots] = value;
            slots[(hashValue + i) % nslots] = new string(value);
            nitems++;
            break;
        }
    }
    if ((double)nitems / nslots >= 0.5) {
        rehash();
    }
}

bool HashSet::lookup(const std::string& value) const{
    uint64_t hashValue = strfn->hash(value);
    hashValue = intfn->hash(hashValue);

    for (int i = 0; i < nslots; i++) {
        if (slots[(hashValue + i) % nslots] == NULL) {
            return false;
        }
        else if (*slots[(hashValue + i) % nslots] == value) {
            return true;
        }
    }
    return false;
}
