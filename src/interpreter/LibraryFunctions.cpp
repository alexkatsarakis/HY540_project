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

unsigned whitespace = 0;

#define WHITESPACE_STEP 4


#define SET_RETVAL(x) env.Set(RETVAL_RESERVED_FIELD,(x));

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
        if(env[argNo] == NULL)Interpreter::RuntimeError("Wrong Arguments");
        return env[argNo];
    }

    auto arg = env[optArgName];
    if (!arg) arg = env[argNo];

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
    SET_RETVAL(value->GetTypeToString());
}

void LibFunc::ObjectKeys(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    if (!value->IsObject()) return Interpreter::RuntimeError("Library function \"object_keys\" did not receive an object.");

    const Object * obj = value->ToObject();
    Object * table = new Object();
    table->IncreaseRefCounter();
    unsigned i = 0;

    obj->Visit([table, &i](const Value &key, const Value &val) {
        table->Set(i++, key);
    });

    SET_RETVAL(table);
}

void LibFunc::ObjectSize(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    if (!value->IsObject()) return Interpreter::RuntimeError("Library function \"object_size\" did not receive an object.");

    const Object * obj = value->ToObject();
    SET_RETVAL(static_cast<double>(obj->GetTotal()));
}

void LibFunc::Sleep(Object & env) {
    assert(env.IsValid());

    const Value * value = GetArgument(env, 0);
    if (!value->IsNumber()) return Interpreter::RuntimeError("Library function \"sleep\" did not receive a number.");
    if (!Utilities::IsInt(value->ToNumber())) return Interpreter::RuntimeError("Library function \"sleep\" did not receive an integer.");

    int number = static_cast<int>(value->ToNumber());

    std::this_thread::sleep_for(std::chrono::milliseconds(number));

    SET_RETVAL(Value());
}





void LibFunc::Assert(Object & env) {
    assert(env.IsValid());

    for(register unsigned i = 0; i < env.GetNumericSize(); ++i) {
        const auto& value = GetArgument(env, i);
        if(value->GetType() == Value::Type::BooleanType){
            if(!value->ToBoolean())Interpreter::Assert("Boolean Not True");
        }
        if(value->GetType() == Value::Type::NilType ||
           value->GetType() == Value::Type::UndefType){
            Interpreter::Assert("Undefined Value");
        }
    }
    SET_RETVAL(true);
}

void LibFunc::Sqrt(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(value->GetType() != Value::Type::NumberType)Interpreter::RuntimeError("You can only square a number");
    SET_RETVAL(sqrt(value->ToNumber()));
}

void LibFunc::Pow(Object & env) {
    assert(env.IsValid());
    const auto& value1 = GetArgument(env, 0);
    const auto& value2 = GetArgument(env, 1);
    if(value1->GetType() != Value::Type::NumberType || value2->GetType() != Value::Type::NumberType)Interpreter::RuntimeError("You can only power a number");
    SET_RETVAL(pow(value1->ToNumber(),value2->ToNumber()));
}

void LibFunc::Sin(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(value->GetType() != Value::Type::NumberType)Interpreter::RuntimeError("You can only use sin of a number");
    SET_RETVAL(sin(value->ToNumber()));
}

void LibFunc::Cos(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(value->GetType() != Value::Type::NumberType)Interpreter::RuntimeError("You can only use cos of a number");
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
    std::cin >> input;
    if(!input.empty() && isNumber(input)){ 
        SET_RETVAL(std::stod(input));
    }else{
        SET_RETVAL(input);
    }
}

void LibFunc::Random(Object & env) {
    assert(env.IsValid());
    SET_RETVAL(static_cast<double>(std::rand())/ RAND_MAX);
}

void LibFunc::ToNumber(Object & env) {
    assert(env.IsValid());
    const auto& value = GetArgument(env, 0);
    if(value->GetType() == Value::Type::NumberType){
        SET_RETVAL(value->ToNumber());
        return;
    }
    if(value->GetType() != Value::Type::StringType)Interpreter::RuntimeError("You can only use to_number with a string or a number");
    if(value->ToString().empty() || !isNumber(value->ToString()))Interpreter::RuntimeError("The given string isn't a number");

    SET_RETVAL(std::stod(value->ToString()));
    
}

void LibFunc::FileOpen(Object & env) {
    assert(env.IsValid());
    const auto& path = GetArgument(env, 0);
    const auto& mode = GetArgument(env, 1);
    
    if(path->GetType() != Value::Type::StringType)Interpreter::RuntimeError("First argument in file open should be a string with the path");
    if(mode->GetType() != Value::Type::StringType)Interpreter::RuntimeError("Second argument in file open should be a string with the mode");

    auto* fp = fopen(path->ToString().c_str(),mode->ToString().c_str());
    if(fp){
        SET_RETVAL(Value(fp, "EXTERNAL_FILE"));
    }else{
        SET_RETVAL(Value());
    }
}

void LibFunc::FileClose(Object & env) {
    assert(env.IsValid());
    const auto& fp = GetArgument(env, 0);
    
    if(fp->GetType() != Value::Type::NativePtrType)Interpreter::RuntimeError("First argument in file_close is ...");
    if(fp->ToNativeTypeId() != "EXTERNAL_FILE")Interpreter::RuntimeError("First argument in file_close is not a file");
    
    fclose(static_cast<FILE*>(fp->ToNativePtr()));
    //FREE FP

    SET_RETVAL(true);
}

void LibFunc::FileWrite(Object & env) {
    assert(env.IsValid());
    const auto& fp = GetArgument(env, 0);
    const auto& value = GetArgument(env, 1);
    
    if(fp->GetType() != Value::Type::NativePtrType)Interpreter::RuntimeError("First argument in file_write is ...");
    if(fp->ToNativeTypeId() != "EXTERNAL_FILE")Interpreter::RuntimeError("First argument in file_write is not a file");
    
    if(value->GetType() != Value::Type::StringType)Interpreter::RuntimeError("Second argument in file_write must be a string");
    
    fprintf(static_cast<FILE*>(fp->ToNativePtr()),"%s",value->ToString().c_str());
    
    SET_RETVAL(true);
}

void LibFunc::FileRead(Object & env) {
    assert(env.IsValid());
    const auto& fp = GetArgument(env, 0);
    
    if(fp->GetType() != Value::Type::NativePtrType)Interpreter::RuntimeError("First argument in file_write is ...");
    if(fp->ToNativeTypeId() != "EXTERNAL_FILE")Interpreter::RuntimeError("First argument in file_write is not a file");
    
    const auto& filePtr = static_cast<FILE*>(fp->ToNativePtr());
    fseek(filePtr, 0, SEEK_END); 
    unsigned long size = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
    char* content = static_cast<char*>(malloc(sizeof(char)*size));
    fread(content, 1, size, filePtr);
    
    SET_RETVAL(content);
    free(content);
}