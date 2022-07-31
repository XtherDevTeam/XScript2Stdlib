#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

XScript::EnvClassObject *
ConstructInternalErrorStructure(XScript::BytecodeInterpreter *Interpreter, const XScript::XString &ErrorName,
                                const XScript::XString &ErrorDescription);

XScript::EnvClassObject *CloneFSObject(XScript::BytecodeInterpreter *Interpreter);

void open(XScript::ParamToMethod Param);

void close(XScript::ParamToMethod Param);

void read(XScript::ParamToMethod Param);

void write(XScript::ParamToMethod Param);

extern "C" XScript::NativeClassInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
