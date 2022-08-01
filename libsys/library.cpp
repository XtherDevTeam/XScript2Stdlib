#include <iostream>

#include "library.h"

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"time")] = {0, sys_time};
    Methods[XScript::Hash(L"exit")] = {0, sys_exit};
    Methods[XScript::Hash(L"boot_time_ms")] = {0, sys_boot_time_ms};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"System")] = {L"System", Methods};
    return {
            L"Jerry Chou",
            L"XScript 2 LibSystem",
            Classes};
}

void sys_time(XScript::ParamToMethod Param) {
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

void sys_exit(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    EnvironmentStackItem Code = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    Interpreter->InterpreterEnvironment->Heap.~EnvironmentHeap();
    exit(Code.Value.IntVal);
}

void sys_boot_time_ms(XScript::ParamToMethod Param) {
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
