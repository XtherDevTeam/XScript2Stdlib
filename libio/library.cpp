#include <iostream>

#include "library.h"


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"println")] = {0, println};
    Methods[XScript::Hash(L"readInt")] = {0, readInt};
    Methods[XScript::Hash(L"readDeci")] = {0, readDeci};
    Methods[XScript::Hash(L"readBool")] = {0, readBool};
    Methods[XScript::Hash(L"readStr")] = {0, readStr};
    return {
            L"Jerry Chou",
            L"XScript 2 LibIO",
            Methods};
}

void println(XScript::ParamToMethod Param) {
    using namespace XScript;
    BytecodeInterpreter Interpreter(*static_cast<Environment *>(Param.VMPointer),
                                    *static_cast<GarbageCollection *>(Param.VMGC));

    EnvironmentStackItem Item = static_cast<Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    auto StringToPrint = static_cast<Environment *>(Param.VMPointer)->Heap.HeapData[
            static_cast<Environment *>(Param.VMPointer)->Heap.HeapData[Item.Value.HeapPointerVal].Value.ClassObjectPointer->Members[Hash(
                    L"__buffer__")]].Value.StringObjectPointer;

    XBytes Str{wstring2string(&StringToPrint->Dest)};
    printf("%s\n", Str.c_str());

    Interpreter.InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {}});
}

void readInt(XScript::ParamToMethod Param) {
    using namespace XScript;
    BytecodeInterpreter Interpreter(*static_cast<Environment *>(Param.VMPointer),
                                    *static_cast<GarbageCollection *>(Param.VMGC));

    XInteger Int;
    std::cin >> Int;
    Interpreter.InterpreterEnvironment.Stack.PushValueToStack({EnvironmentStackItem::ItemKind::Integer,
                                                               (EnvironmentStackItem::ItemValue) Int});
    Interpreter.InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {}});
}

void readBool(XScript::ParamToMethod Param) {
    using namespace XScript;
    BytecodeInterpreter Interpreter(*static_cast<Environment *>(Param.VMPointer),
                                    *static_cast<GarbageCollection *>(Param.VMGC));

    XBoolean Int;
    std::cin >> Int;
    Interpreter.InterpreterEnvironment.Stack.PushValueToStack({EnvironmentStackItem::ItemKind::Boolean,
                                                               (EnvironmentStackItem::ItemValue) Int});
    Interpreter.InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {}});
}

void readDeci(XScript::ParamToMethod Param) {
    using namespace XScript;
    BytecodeInterpreter Interpreter(*static_cast<Environment *>(Param.VMPointer),
                                    *static_cast<GarbageCollection *>(Param.VMGC));

    XDecimal Deci;
    std::cin >> Deci;
    Interpreter.InterpreterEnvironment.Stack.PushValueToStack({EnvironmentStackItem::ItemKind::Decimal,
                                                               (EnvironmentStackItem::ItemValue) Deci});
    Interpreter.InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {}});
}

void readStr(XScript::ParamToMethod Param) {
    using namespace XScript;
    BytecodeInterpreter Interpreter(*static_cast<Environment *>(Param.VMPointer),
                                    *static_cast<GarbageCollection *>(Param.VMGC));

    XBytes Str;
    std::cin >> Str;
    XHeapIndexType Idx = Interpreter.InterpreterEnvironment.Heap.PushElement(
            (EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                         (EnvObject::ObjectValue) CreateEnvStringObjectFromXString(string2wstring(Str))}
    );

    Interpreter.InterpreterEnvironment.Stack.PushValueToStack({EnvironmentStackItem::ItemKind::HeapPointer,
                                                               (EnvironmentStackItem::ItemValue) Idx});
    Interpreter.InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {}});
}
