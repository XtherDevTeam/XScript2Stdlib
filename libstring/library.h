#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

// 非 export 部分

void FuncReturn(XScript::Environment *Env);

/**
 * 从Stack上的`this`指针克隆一个String对象 用于生成对象
 * @param Env VM环境
 */
XScript::EnvClassObject * CloneStringObject(XScript::Environment *Env);

// export 部分

void fromBuffer(XScript::ParamToMethod Param);

void fromInt(XScript::ParamToMethod Param);

void fromDeci(XScript::ParamToMethod Param);

void fromBool(XScript::ParamToMethod Param);

void startsWith(XScript::ParamToMethod Param);

void endsWith(XScript::ParamToMethod Param);

extern "C" XScript::NativeClassInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
