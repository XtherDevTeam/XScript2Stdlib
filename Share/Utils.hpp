//
// Created by Jerry Chou on 2022/8/6.
//

#ifndef XSCRIPT2STDLIB_UTILS_HPP
#define XSCRIPT2STDLIB_UTILS_HPP

#include "../XScript2/Executor/EnvStringObject.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

XScript::EnvClassObject *
ConstructInternalErrorStructure(XScript::BytecodeInterpreter *Interpreter, const XScript::XString &ErrorName,
                                const XScript::XString &ErrorDescription);

void PushClassObjectStructure(XScript::BytecodeInterpreter *Interpreter, XScript::EnvClassObject *Object);

XScript::XHeapIndexType CloneObject(XScript::BytecodeInterpreter *Interpreter, XScript::XHeapIndexType Target);

XScript::EnvStringObject *GetStringObject(XScript::BytecodeInterpreter &Interpreter, const XScript::EnvironmentStackItem &Item);

XScript::EnvBytesObject * GetBytesObject(XScript::BytecodeInterpreter &Interpreter, const XScript::EnvironmentStackItem &Item);

XScript::XString deci2bin(XScript::XDecimal Dec);

#define builtin_hashcode___buffer__ (XScript::XIndexType)17751072636344391196

#endif //XSCRIPT2STDLIB_UTILS_HPP
