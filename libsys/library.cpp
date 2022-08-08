#include <iostream>

#include "library.h"
#include "../Share/Utils.hpp"

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"time")] = {0, System_time};
    Methods[XScript::Hash(L"exit")] = {0, System_exit};
    Methods[XScript::Hash(L"boot_time_ms")] = {0, System_boot_time_ms};
    Methods[XScript::Hash(L"GC")] = {0, System_GC};
    Methods[XScript::Hash(L"clone")] = {0, System_clone};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"System")] = {L"System", Methods};
    return {
            L"Jerry Chou",
            L"XScript 2 LibSystem",
            Classes};
}

void System_time(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    EnvironmentStackItem::ItemKind::Integer,
                    (EnvironmentStackItem::ItemValue)
                            static_cast<XInteger>(std::chrono::duration_cast<std::chrono::seconds>(
                                    std::chrono::system_clock::now().time_since_epoch()).count())
            });
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void System_exit(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    EnvironmentStackItem Code = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    Interpreter->InterpreterEnvironment->Heap.~EnvironmentHeap();
    exit(Code.Value.IntVal);
}

void System_boot_time_ms(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    EnvironmentStackItem::ItemKind::Integer,
                    (EnvironmentStackItem::ItemValue)
                            static_cast<XInteger>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::steady_clock::now().time_since_epoch()).count())
            });
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void System_GC(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->GC->Start(true);
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void System_clone(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    EnvironmentStackItem ToClone = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    if (ToClone.Kind == XScript::EnvironmentStackItem::ItemKind::HeapPointer) {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        EnvironmentStackItem::ItemKind::HeapPointer,
                        (EnvironmentStackItem::ItemValue) CloneObject(Interpreter, ToClone.Value.HeapPointerVal)});
    } else {
        PushClassObjectStructure(
                Interpreter,
                ConstructInternalErrorStructure(Interpreter, L"SystemError", L"Expected a heap pointer"));
    }
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}
