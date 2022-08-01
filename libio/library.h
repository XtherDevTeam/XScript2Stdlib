#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

void FuncReturn(XScript::Environment *Env);

void println(XScript::ParamToMethod Param);

void readInt(XScript::ParamToMethod Param);

void readBool(XScript::ParamToMethod Param);

void readDeci(XScript::ParamToMethod Param);

void readStr(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
