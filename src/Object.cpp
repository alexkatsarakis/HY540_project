#include "Object.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cassert>

Object::Object() {
    refCounter = 0;
    numMap.clear();
    strMap.clear();
}

Object::Object (const Object & obj) {
    assert(obj.IsValid());

    numMap.clear();
    strMap.clear();
    numMap = obj.numMap;
    strMap = obj.strMap;

    /* TODO: Is the assignment the correct way to do it? Perhaps the following
     * is a better solution?
     *
     * numMap.insert(obj.numMap.begin(); obj.numMap.end());
     */
}

/****** Verifier ******/

bool Object::IsValid(void) const {
    for(auto & pair : numMap) {
        bool valid = pair.second && pair.second->IsValid();
        if (!valid) return false;
    }

    for(auto & pair : strMap) {
        bool valid = !pair.first.empty() && pair.second && pair.second->IsValid();
        if (!valid) return false;
    }

    return true;
}

/****** Observers ******/

bool Object::ElementExists(double key) const {
    assert(IsValid());
    return (numMap.find(key) != numMap.end());
}

bool Object::ElementExists(const std::string & key) const {
    assert(IsValid());
    assert(!key.empty());  /* TODISCUSS: Should we allow the following: obj[""] */
    return (strMap.find(key) != strMap.end());
}

unsigned Object::GetTotal(void) const {
    assert(IsValid());
    return numMap.size() + strMap.size();
}

/****** Modifiers ******/

void Object::IncreaseRefCounter(void) { refCounter += 1; }

void Object::DecreaseRefCounter(void) {
    assert(refCounter > 0);
    refCounter -= 1;
}

/****** Getters ******/

const Value * Object::operator[] (double key) const{
    assert(IsValid());

    auto iterator = numMap.find(key);

    if (iterator == numMap.end()) return nullptr;
    else return iterator->second;
}

const Value * Object::operator[] (const std::string & key) const {
    assert(IsValid());
    assert(!key.empty());  /* TODISCUSS: Should we allow the following: obj[""] */

    auto iterator = strMap.find(key);

    if (iterator == strMap.end()) return nullptr;
    else return iterator->second;
}

/****** Setters ******/

void Object::Set(double key, const Value & value) {
    assert(value.IsValid());
    assert(IsValid());

    auto val = new Value(value);  /* TODISCUSS: Should values be copied */
    numMap.insert(std::pair<double, ValuePtr>(key, val));

    assert(numMap.size() > 0);
    assert(IsValid());
}

void Object::Set(const std::string & key, const Value & value) {
    assert(value.IsValid());
    assert(IsValid());

    auto val = new Value(value);
    strMap.insert(std::pair<std::string, ValuePtr>(key, val));

    assert(strMap.size() > 0);
    assert(IsValid());
}

void Object::Remove(double key) {
    assert(IsValid());
    assert(ElementExists(key));

    unsigned oldSize = numMap.size();
    numMap.erase(numMap.find(key));

    assert(numMap.size() == oldSize - 1);
    assert(IsValid());
}

void Object::Remove(const std::string & key) {
    assert(IsValid());
    assert(ElementExists(key));

    unsigned oldSize = strMap.size();
    strMap.erase(strMap.find(key));

    assert(strMap.size() == oldSize - 1);
    assert(IsValid());
}

/****** Visitors ******/

void Object::Visit(const Visitor & func) const {
    assert(func);
    assert(IsValid());

    for(auto & pair : numMap) func(pair.first, *pair.second);
    for(auto & pair : strMap) func(pair.first, *pair.second);
}

void Object::Apply(const Applier & func) {
    assert(func);
    assert(IsValid());

    for(auto & pair : numMap) func(pair.first, *pair.second);
    for(auto & pair : strMap) func(pair.first, *pair.second);
}

/****** Destructor ******/

Object::~Object() {
    assert(IsValid());
}