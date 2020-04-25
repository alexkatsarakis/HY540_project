#include "Interpreter.h"
#include "LibraryFunctions.h"
#include "TreeTags.h"
#include "Utilities.h"

#include <cmath>

#include <iostream>
#include <cassert>
#include <ostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <fstream>

unsigned whitespace = 0;

#define WHITESPACE_STEP 4


#define NAT_POINTER_EXTERNAL_FILE "EXTERNAL_FILE_PTR"
#define SET_RETVAL(x) env.Set(RETVAL_RESERVED_FIELD,(x));
#define THROW_WRONG_ARGUMENT Interpreter::RuntimeError("Wrong Argument Value");

void PrintNumber(std::ostream & stream, const Value * value);
void PrintString(std::ostream & stream, const Value * value);
void PrintBoolean(std::ostream & stream, const Value * value);
void PrintUndef(std::ostream & stream, const Value * value);
void PrintNil(std::ostream & stream, const Value * value);
void PrintLibraryFunction(std::ostream & stream, const Value * value);
void PrintNativePointer(std::ostream & stream, const Value * value);
void PrintProgramFunction(std::ostream & stream, const Value * value);
void PrintObject(std::ostream & stream, const Value * value);
void PrintValue(std::ostream & stream, const Value * value);

const Value * GetArgument(Object & env, unsigned argNo, const std::string & optArgName = "") {
    assert(env.IsValid());

    if (optArgName.empty()){
        if (!env[argNo]) Interpreter::RuntimeError("Library function did not receive argument " + std::to_string(argNo + 1));
        return env[argNo];
    }

    const Value * arg = env[optArgName];
    if (!arg) arg = env[argNo];

    Interpreter::RuntimeError("Library function did not receive valid argument");

    return arg;
}

void PrintNumber(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsNumber());
    stream << (Utilities::IsInt(value->ToNumber()) ? static_cast<int>(value->ToNumber()) : value->ToNumber()) << std::endl;
}

void PrintString(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsString());
    stream << value->ToString() << std::endl;
}

void PrintBoolean(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsBoolean());
    stream << (value->ToBoolean() ? "true" : "false") << std::endl;
}

void PrintUndef(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsUndef());
    stream << "undef" << std::endl;
}

void PrintNil(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsNil());
    stream << "nil" << std::endl;
}

void PrintLibraryFunction(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsLibraryFunction());
    stream << "[ Library Function \"" << value->ToLibraryFunctionId() << "\" ]" << std::endl;
}

void PrintNativePointer(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsNativePtr());
    stream << "[ Pointer " << value->ToNativeTypeId() << " ]" << std::endl;
}

void PrintProgramFunction(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsProgramFunction());

    const Object * funcdef = value->ToProgramFunctionAST();
    assert((*funcdef)[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_FUNCTION_DEF);

    const Object * child = (*funcdef)[AST_TAG_FUNCTION_ID]->ToObject();
    assert((*child)[AST_TAG_TYPE_KEY]->ToString() == AST_TAG_ID);

    std::string name = (*child)[AST_TAG_ID]->ToString();

    if(name[0] == '$') stream << "[ Anonymous Function ]" << std::endl;
    else stream << "[ Function \"" + name + "\" ]" << std::endl;
}

void PrintObject(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsObject());

    Object * o = value->ToObject_NoConst();

    if(o->GetTotal() == 0) {
        stream << "[Object] [ ]" << std::endl;
        return;
    }

    stream << "[Object] [" << std::endl;

    whitespace += WHITESPACE_STEP;
    o->Visit([&stream](const Value &key, const Value &val) {
        if (key.IsString()) stream << std::setw(whitespace) << '"' << key.ToString() << "\" : ";
        else if (key.IsNumber()) stream << std::setw(whitespace) << key.ToNumber() << " : ";
        else assert(false);

        PrintValue(stream, &val);
    });
    whitespace -= WHITESPACE_STEP;

    stream << std::setw(whitespace) << "]" << std::endl;
}

void PrintValue(std::ostream & stream, const Value * value) {
    assert(stream.good());
    assert(value && value->IsValid());

    switch(value->GetType()) {
        case Value::Type::NumberType: return PrintNumber(stream, value);
        case Value::Type::StringType: return PrintString(stream, value);
        case Value::Type::BooleanType: return PrintBoolean(stream, value);
        case Value::Type::UndefType: return PrintUndef(stream, value);
        case Value::Type::NilType: return PrintNil(stream, value);
        case Value::Type::LibraryFunctionType: return PrintLibraryFunction(stream, value);
        case Value::Type::NativePtrType: return PrintNativePointer(stream, value);
        case Value::Type::ProgramFunctionType: return PrintProgramFunction(stream, value);
        case Value::Type::ObjectType: return PrintObject(stream, value);
        default: assert(false);
    }
}

void LibFunc::Print(Object & env) {
    assert(env.IsValid());

    for(register unsigned i = 0; i < env.GetNumericSize(); ++i) {
        const Value * value = GetArgument(env, i);
        PrintValue(std::cout, value);
    }

    SET_RETVAL(Value());
}

void LibFunc::Typeof(Object & env) {
    assert(env.IsValid());
    const Value * value = GetArgument(env, 0);
    assert(value);
    SET_RETVAL(value->GetTypeToString());
}

void LibFunc::ObjectKeys(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    assert(value);
    if (!value->IsObject()) return Interpreter::RuntimeError("Library function \"object_keys\" received a " + value->GetTypeToString() + " instead of an object");

    const Object * obj = value->ToObject();
    Object * table = new Object();
    unsigned index = 0;

    obj->Visit([table, &index](const Value &key, const Value &val) {
        table->Set(index++, key);
    });

    SET_RETVAL(table);
}

void LibFunc::ObjectSize(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    assert(value);
    if (!value->IsObject()) return Interpreter::RuntimeError("Library function \"object_size\" received a " + value->GetTypeToString() + " instead of an object");

    const Object * obj = value->ToObject();
    SET_RETVAL(static_cast<double>(obj->GetTotal()));
}

void LibFunc::Sleep(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    assert(value);

    if (!value->IsNumber()) return Interpreter::RuntimeError("Library function \"sleep\" did not receive a number");
    if (!Utilities::IsInt(value->ToNumber())) return Interpreter::RuntimeError("Library function \"sleep\" did not receive an integer");

    int number = static_cast<int>(value->ToNumber());

    std::this_thread::sleep_for(std::chrono::milliseconds(number));

    SET_RETVAL(Value());
}

void LibFunc::Assert(Object & env) {
    for(register unsigned i = 0; i < env.GetNumericSize(); ++i) {
        const Value * value = GetArgument(env, i);
        assert(value);
        if (!value->operator bool()) Interpreter::Assert("Value of " + value->GetTypeToString() + " was evaluated to false");
    }
    SET_RETVAL(true);
}

void LibFunc::Sqrt(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(!value->IsNumber())THROW_WRONG_ARGUMENT;
    SET_RETVAL(sqrt(value->ToNumber()));
}

void LibFunc::Pow(Object & env) {
    assert(env.IsValid());
    const auto& value1 = GetArgument(env, 0);
    const auto& value2 = GetArgument(env, 1);
    if(!value1->IsNumber())THROW_WRONG_ARGUMENT;
    if(!value2->IsNumber())THROW_WRONG_ARGUMENT;
    SET_RETVAL(pow(value1->ToNumber(),value2->ToNumber()));
}

void LibFunc::Sin(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(!value->IsNumber())THROW_WRONG_ARGUMENT;
    SET_RETVAL(sin(value->ToNumber()));
}

void LibFunc::Cos(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(!value->IsNumber())THROW_WRONG_ARGUMENT;
    SET_RETVAL(cos(value->ToNumber()));
}

void LibFunc::GetTime(Object & env) {
    assert(env.IsValid());
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::string conv = std::to_string(now->tm_mday) + "/" + std::to_string(now->tm_mon) + "/" + std::to_string(1900+now->tm_year);
    conv += " " + std::to_string(now->tm_hour) +":"+ std::to_string(now->tm_min) +":"+ std::to_string(now->tm_sec);
    SET_RETVAL(conv);
}

bool isNumber(std::string str){
    unsigned int i = 0;
    if(str[0] == '-')i++;
    while(i < str.length()){
        if(isdigit(str[i]) == false && str[i] != '.')return false;
        i++;
    }
    return true;
}

void LibFunc::Input(Object & env) {
    assert(env.IsValid());
    std::string input;
    std::getline(std::cin, input);
    if(!input.empty() && isNumber(input)){
        SET_RETVAL(std::stod(input));
    }else{
        SET_RETVAL(input);
    }
}

void SeedRNG(void) {
 unsigned seed = time(NULL);

    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (urandom)
        urandom.read(reinterpret_cast<char *>(&seed), sizeof(seed));

    std::srand(seed);
}

void LibFunc::Random(Object & env) {
    assert(env.IsValid());

    static unsigned char callCount = 0;
    if (!callCount) {
        callCount = 1;
        SeedRNG();
    }

    SET_RETVAL(static_cast<double>(std::rand())/ RAND_MAX);
}

void LibFunc::ToNumber(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(value->IsNumber()){
        SET_RETVAL(value->ToNumber());
        return;
    }
    if(!value->IsString())THROW_WRONG_ARGUMENT;
    if(value->ToString().empty() || !isNumber(value->ToString()))THROW_WRONG_ARGUMENT;

    SET_RETVAL(std::stod(value->ToString()));
    
}

void LibFunc::FileOpen(Object & env) {
    assert(env.IsValid());
    const auto& path = GetArgument(env, 0);
    const auto& mode = GetArgument(env, 1);
    
    if(!path->IsString())THROW_WRONG_ARGUMENT;
    if(!mode->IsString())THROW_WRONG_ARGUMENT;

    auto* fp = fopen(path->ToString().c_str(),mode->ToString().c_str());
    if(fp){
        SET_RETVAL(Value(fp, NAT_POINTER_EXTERNAL_FILE));
    }else{
        SET_RETVAL(Value());
    }
}

void LibFunc::FileClose(Object & env) {
    assert(env.IsValid());
    const auto& fp = GetArgument(env, 0);
    
    if(!fp->IsNativePtr())THROW_WRONG_ARGUMENT;
    if(fp->ToNativeTypeId() != NAT_POINTER_EXTERNAL_FILE)THROW_WRONG_ARGUMENT;
    
    fclose(static_cast<FILE*>(fp->ToNativePtr()));
    //FREE FP //TODO

    SET_RETVAL(true);
}

void LibFunc::FileWrite(Object & env) {
    assert(env.IsValid());
    const auto& fp = GetArgument(env, 0);
    const auto& value = GetArgument(env, 1);
    
    if(!fp->IsNativePtr())THROW_WRONG_ARGUMENT;
    if(fp->ToNativeTypeId() != NAT_POINTER_EXTERNAL_FILE)THROW_WRONG_ARGUMENT;
    if(!value->IsString())THROW_WRONG_ARGUMENT;
    
    fprintf(static_cast<FILE*>(fp->ToNativePtr()),"%s",value->ToString().c_str());
    
    SET_RETVAL(true);
}

void LibFunc::FileRead(Object & env) {
    assert(env.IsValid());
    const auto& fp = GetArgument(env, 0);
    
    if(!fp->IsNativePtr())THROW_WRONG_ARGUMENT;
    if(fp->ToNativeTypeId() != NAT_POINTER_EXTERNAL_FILE)THROW_WRONG_ARGUMENT;
    
    const auto& filePtr = static_cast<FILE*>(fp->ToNativePtr());
    fseek(filePtr, 0, SEEK_END); 
    unsigned long size = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
    char* content = static_cast<char*>(malloc(sizeof(char)*size));
    unsigned long readSize(fread(content, 1, size, filePtr));
    assert(readSize == size);
    
    SET_RETVAL(content);
    free(content);
}