#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

void Math_e(XScript::ParamToMethod Param);

void Math_pi(XScript::ParamToMethod Param);

void Math_log2e(XScript::ParamToMethod Param);

void Math_log10e(XScript::ParamToMethod Param);

void Math_sqrt2(XScript::ParamToMethod Param);

void Math_sqrt3(XScript::ParamToMethod Param);

void Math_sqrt(XScript::ParamToMethod Param);

void Math_floor(XScript::ParamToMethod Param);

void Math_ceil(XScript::ParamToMethod Param);

void Math_trunc(XScript::ParamToMethod Param);

void Math_gcd(XScript::ParamToMethod Param);

void Math_abs(XScript::ParamToMethod Param);

void Math_sin(XScript::ParamToMethod Param);

void Math_cos(XScript::ParamToMethod Param);

void Math_tan(XScript::ParamToMethod Param);

void Math_asin(XScript::ParamToMethod Param);

void Math_acos(XScript::ParamToMethod Param);

void Math_atan(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
