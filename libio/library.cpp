#include <iostream>

#include "library.h"


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"println")] = {0, println};
    return {
            L"Jerry Chou",
            L"XScript 2 LibIO",
            Methods};
}

void println(XScript::ParamToMethod Param) {
    using namespace XScript;
    BytecodeInterpreter Interpreter(*static_cast<Environment *>(Param.VMPointer), *static_cast<GarbageCollection *>(Param.VMGC));

    EnvironmentStackItem Item = static_cast<Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    auto StringToPrint = static_cast<Environment *>(Param.VMPointer)->Heap.HeapData[
            static_cast<Environment *>(Param.VMPointer)->Heap.HeapData[Item.Value.HeapPointerVal].Value.ClassObjectPointer->Members[Hash(
                    L"__buffer__")]].Value.StringObjectPointer;

    XBytes Str{wstring2string(&StringToPrint->Dest)};
    printf("%s\n", Str.c_str());

    Interpreter.InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {}});
}