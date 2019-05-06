#include "HashSet.h"
#include <iostream>
#include <cstdint>
#include <string>
using namespace std;

void HashSet::rehash(){
    // To store items in the old slots
    string temp[nitems];
    for (int i = 0, j = 0; j < nslots; j++) {
        if(slots[j] != NULL){
            temp[i] = *slots[j];
            i++;
        }
    }
    // To delete the old slots
    for (int i = 0; i < nslots; i++) {
        delete slots[i];
        slots[i] = NULL;
    }
    // To creat a new larger slots
    nslots *= 2;
    slots = new string*[nslots];//
    for (int i = 0; i < nslots; i++) {
        slots[i] = NULL;
    }
    // To hash the items into the new slots
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
    this->slots = new string*[nslots];//
    this->intfn = new DivisionHash(0, nslots);
    this->strfn = new JenkinsHash();
    for (int i = 0; i < nslots; i++) {
        slots[i] = NULL;
    }
}

HashSet::~HashSet(){
    delete intfn;
    intfn = NULL;
    delete strfn;
    strfn = NULL;
    for (int i = 0; i < nslots; i++) {
        delete slots[i];
        slots[i] = NULL;
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
        rehash();//
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
