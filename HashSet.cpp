#include "HashSet.h"
#include <iostream>
#include <cstdint>
#include <string>

void HashSet::rehash(){
    // To store items in the old slots
    std::string temp[nitems];
    for (int i = 0, j = 0; j < nslots; j++) {
        if(slots[j] != NULL){
            temp[i] = *slots[j];
            i++;
        }
    }
    // To delete the old slots
    for (int i = 0; i < nslots; i++) {
        delete slots[i];
    }
    delete [] slots;
    // To creat a new larger slots
    nslots *= 2;
    slots = new std::string*[nslots];
    for (int i = 0; i < nslots; i++) {
        slots[i] = NULL;
    }
    // To hash the items into the new slots
    for (int i = 0; i < nitems; i++) {
        uint64_t key = strfn->hash(temp[i]);
        uint64_t hashValue = intfn->hash(key);
        for (int j = 0; ; j++) {
            if (slots[(hashValue + j) % nslots] == NULL) {
                slots[(hashValue + j) % nslots] = new std::string(temp[i]);
                break;
            }
        }
    }
}

HashSet::HashSet(){
    this->nitems = 0;
    this->nslots = 100;
    this->slots = new std::string*[nslots];
    this->intfn = new DivisionHash((uint64_t)0, (uint64_t)nslots);
    this->strfn = new JenkinsHash();
    for (int i = 0; i < nslots; i++) {
        slots[i] = NULL;
    }
}

HashSet::~HashSet(){
    delete intfn;
    delete strfn;
    for (int i = 0; i < nslots; i++) {
        delete slots[i];
    }
    delete [] slots;
}

void HashSet::insert(const std::string& value){
    uint64_t key = strfn->hash(value);
    uint64_t hashValue = intfn->hash(key);
    for (int i = 0; ; i++) {
        if (slots[(hashValue + i) % nslots] == NULL) {
            slots[(hashValue + i) % nslots] = new std::string(value);
            nitems++;
            break;
        }
    }
    if (2 * nitems >= nslots) {
        rehash();
    }
}

bool HashSet::lookup(const std::string& value) const{
    uint64_t key = strfn->hash(value);
    uint64_t hashValue = intfn->hash(key);
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
