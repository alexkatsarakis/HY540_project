#include "Object.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cassert>

Object::Object() {
    refCounter = 0;
}

bool Object::IsValid(void) const {
    for(auto & pair : numHash) {
        bool valid = pair.second && pair.second->IsValid();
        if (!valid) return false;
    }

    for(auto & pair : stringHash) {
        bool valid = !pair.first.empty() && pair.second && pair.second->IsValid();
        if (!valid) return false;
    }

    return true;
}

void Object::IncreaseRefCounter(void) { refCounter += 1; }

void Object::DecreaseRefCounter(void) {
    assert(refCounter > 0);
    refCounter -= 1;
}

bool Object::ElementExists(unsigned index) const {
    return (numHash.find(index) != numHash.end());
}

bool Object::ElementExists(const std::string & str) const {
    assert(!str.empty());
    return (stringHash.find(str) != stringHash.end());
}

const Value & Object::operator[] (unsigned index) const{
    assert(ElementExists(index));
    return *numHash.at(index);
}

const Value & Object::operator[] (const std::string & str) const {
    assert(!str.empty());
    assert(ElementExists(str));
    return *stringHash.at(str);
}

void Object::SetValue(unsigned index, const Value & val) {
    numHash.insert(std::pair<unsigned, ValuePtr>(index, std::make_shared<Value>(val)));
    assert(numHash.size() > 0);
    assert(IsValid());
}

void Object::SetValue(const std::string & str, const Value & val) {
    stringHash.insert(std::pair<std::string, ValuePtr>(str, std::make_shared<Value>(val)));
    assert(stringHash.size() > 0);
    assert(IsValid());
}


Object::~Object() { }