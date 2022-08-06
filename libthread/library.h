#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

void xscript2_thread_wrapper(XScript::BytecodeInterpreter *Interpreter, XScript::EnvironmentStackItem Func,
                             const XScript::XArray<XScript::EnvironmentStackItem>& Params);;

void start(XScript::ParamToMethod Param);

void id(XScript::ParamToMethod Param);

void getResult(XScript::ParamToMethod Param);

void sleep_ms(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
