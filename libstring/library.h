#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

// 非 export 部分

struct FormatterTagInfo {
    enum class TagKind : XScript::XInteger {
        Str,
        Int,
        Deci,
        Bool,
        Dol
    } Kind;
    enum class TagBS : XScript::XInteger {
        Hex = 16,
        Dec = 10,
        Oct = 8,
        Bin = 2
    } Base;
    XScript::XInteger Precision;

    FormatterTagInfo();

    XScript::XCharacter * Parse(XScript::XCharacter *Str);
};

/**
 * 从Stack上的`this`指针克隆一个String对象 用于生成对象
 * @param Env VM环境
 */
XScript::EnvClassObject *CloneStringObject(XScript::BytecodeInterpreter *Env);

// export 部分

void fromBuffer(XScript::ParamToMethod Param);

void fromBytes(XScript::ParamToMethod Param);

void fromInt(XScript::ParamToMethod Param);

void fromDeci(XScript::ParamToMethod Param);

void fromBool(XScript::ParamToMethod Param);

void startsWith(XScript::ParamToMethod Param);

void endsWith(XScript::ParamToMethod Param);

void find(XScript::ParamToMethod Param);

void substr(XScript::ParamToMethod Param);

void __instruction_add__(XScript::ParamToMethod Param);

void toInt(XScript::ParamToMethod Param);

void toDeci(XScript::ParamToMethod Param);

void toBool(XScript::ParamToMethod Param);

void toBytes(XScript::ParamToMethod Param);

void length(XScript::ParamToMethod Param);

void formatter(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
