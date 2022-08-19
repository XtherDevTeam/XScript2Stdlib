#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

// 非 export 部分

/**
 * 从Stack上的`this`指针克隆一个HashMap对象 用于生成对象
 * @param Env VM环境
 */
XScript::EnvClassObject *CloneHashMapObject(XScript::BytecodeInterpreter *Interpreter);

// export 部分

/**
 * HashMap.create()
 * @function create a hashmap
 */
void HashMap_create(XScript::ParamToMethod Param);

/**
 * HashMap.insert(x, y)
 * @function insert a pair
 * @param x Key. Expected Integer, String, Bytes object
 * @param y Value. Expected any
 */
void HashMap_insert(XScript::ParamToMethod Param);

/**
 * HashMap.erase(x)
 * @function remove a pair
 * @param x Key. Expected Integer, String, Bytes object
 */
void HashMap_erase(XScript::ParamToMethod Param);

/**
 * HashMap[x]
 * @function Find a pair, return its value.
 * @param x Key. Expected Integer, String, Bytes object
 */
void HashMap___instruction_indexOf__(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
