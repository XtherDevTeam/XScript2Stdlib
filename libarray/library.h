#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

XScript::EnvClassObject * CloneArrayObject(XScript::BytecodeInterpreter *Env);

// export part

void fromBuffer(XScript::ParamToMethod Param);

void push_back(XScript::ParamToMethod Param);

void pop_back(XScript::ParamToMethod Param);

void removeIndex(XScript::ParamToMethod Param);

void resize(XScript::ParamToMethod Param);

void create(XScript::ParamToMethod Param);

void length(XScript::ParamToMethod Param);

void __instruction_indexOf__(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
