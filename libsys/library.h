#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

void System_boot_time_ms(XScript::ParamToMethod Param);

void System_time(XScript::ParamToMethod Param);

void System_exit(XScript::ParamToMethod Param);

void System_GC(XScript::ParamToMethod Param);

void System_clone(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
