#include "Object.h"

#include "TreeTags.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>

Object::Object() {
    refCounter = 0;
    numMap.clear();
    strMap.clear();
}

Object::Object(const Object &obj) {
    assert(obj.IsValid());

    refCounter = obj.refCounter;
    numMap.clear();
    strMap.clear();
    numMap = obj.numMap;
    strMap = obj.strMap;

    assert(IsValid());
}

/****** Verifier ******/

bool Object::IsValid(void) const {
    for (auto &pair : numMap) {
        bool valid = pair.second && pair.second->IsValid();
        if (!valid) return false;
    }

    for (auto &pair : strMap) {
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

bool Object::ElementExists(const std::string &key) const {
    assert(IsValid());
    assert(!key.empty());
    return (strMap.find(key) != strMap.end());
}

unsigned Object::GetTotal(void) const {
    assert(IsValid());
    return numMap.size() + strMap.size();
}

unsigned Object::GetNumericSize(void) const {
    assert(IsValid());
    return numMap.size();
}

unsigned Object::GetStringSize(void) const {
    assert(IsValid());
    return strMap.size();
}

std::vector<std::string> Object::GetUserKeys(void) const {
    assert(IsValid());
    std::vector<std::string> keys;
    for (const auto &pair : strMap) {
        // if (pair.first == AST_TAG_TYPE_KEY) continue;
        if (pair.first.at(0) == '$') continue;
        keys.push_back(pair.first);
    }
    return keys;
}

unsigned Object::GetReferences(void) const {
    assert(IsValid());
    return refCounter;
}

/****** Modifiers ******/

void Object::IncreaseRefCounter(void) { refCounter += 1; }

void Object::DecreaseRefCounter(void) {
    assert(refCounter > 0);
    refCounter -= 1;
    if (refCounter == 0) {
        Clear();
        delete this;
    }
}

const Value *Object::GetAndRemove(double key) {
    assert(IsValid());

    auto pair = numMap.find(key);
    if (pair == numMap.end()) return nullptr;
    unsigned oldSize = numMap.size();
    const Value * value = pair->second;
    numMap.erase(pair);

    assert(numMap.size() == oldSize - 1);
    assert(IsValid());
    return value;
}

const Value *Object::GetAndRemove(const std::string &key) {
    assert(!key.empty());
    assert(IsValid());

    auto pair = strMap.find(key);
    if (pair == strMap.end()) return nullptr;
    unsigned oldSize = strMap.size();
    const Value * value = pair->second;
    strMap.erase(pair);

    assert(strMap.size() == oldSize - 1);
    assert(IsValid());
    return value;
}

void Object::Clear(void) {
    assert(IsValid());

    for (auto &pair : numMap) delete pair.second;
    numMap.clear();

    for (auto &pair : strMap) delete pair.second;
    strMap.clear();

    assert(IsValid());
}

/****** Getters ******/

const Value *Object::operator[](double key) const {
    assert(IsValid());

    auto iterator = numMap.find(key);

    if (iterator == numMap.end())
        return nullptr;
    else
        return iterator->second;
}

const Value *Object::operator[](const std::string &key) const {
    assert(IsValid());
    assert(!key.empty());

    auto iterator = strMap.find(key);

    if (iterator == strMap.end())
        return nullptr;
    else
        return iterator->second;
}

/****** Setters ******/

void Object::Set(double key, const Value &value) {
    assert(value.IsValid());
    assert(IsValid());

    auto val = new Value(value);
    if(numMap.find(key) != numMap.end()) delete numMap[key];
    numMap[key] = val;

    assert(numMap.size() > 0);
    assert(IsValid());
}

void Object::Set(const std::string &key, const Value &value) {
    assert(!key.empty());
    assert(value.IsValid());
    assert(IsValid());

    auto val = new Value(value);
    if(strMap.find(key) != strMap.end()) delete strMap[key];
    strMap[key] = val;

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

void Object::Remove(const std::string &key) {
    assert(!key.empty());
    assert(IsValid());
    assert(ElementExists(key));

    unsigned oldSize = strMap.size();
    strMap.erase(strMap.find(key));

    assert(strMap.size() == oldSize - 1);
    assert(IsValid());
}

/****** Visitors ******/

void Object::Visit(const Visitor &func) const {
    assert(func);
    assert(IsValid());

    for (auto &pair : numMap) func(pair.first, *pair.second);
    for (auto &pair : strMap) func(pair.first, *pair.second);
}

void Object::Apply(const Applier &func) {
    assert(func);
    assert(IsValid());

    for (auto &pair : numMap) func(pair.first, *pair.second);
    for (auto &pair : strMap) func(pair.first, *pair.second);
}

/****** Destructor ******/

Object::~Object() {
    numMap.clear();
    strMap.clear();
    refCounter = 0;
}
