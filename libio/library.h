#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

void FuncReturn(XScript::Environment *Env);

void println(XScript::ParamToMethod Param);

extern "C" XScript::NativeClassInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
