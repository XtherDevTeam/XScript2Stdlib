#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

void sys_time_ms(XScript::ParamToMethod Param);

void sys_exit(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
