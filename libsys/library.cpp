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
    Methods[XScript::Hash(L"version")] = {0, System_version};
    Methods[XScript::Hash(L"sizeOfBuffer")] = {0, System_sizeOfBuffer};
    Methods[XScript::Hash(L"currentHeapSize")] = {0, System_currentHeapSize};
    Methods[XScript::Hash(L"getOptionalData")] = {0, System_getOptionalData};
    Methods[XScript::Hash(L"setOptionalData")] = {0, System_setOptionalData};

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

void System_version(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {XScript::EnvObject::ObjectKind::StringObject,
             (EnvObject::ObjectValue) CreateEnvStringObjectFromXString(CommitVersion)});
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::HeapPointer, (EnvironmentStackItem::ItemValue) Idx});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void System_sizeOfBuffer(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto &Buf = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.HeapPointerVal];
    switch (Buf.Kind) {
        case XScript::EnvObject::ObjectKind::StringObject:
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::Integer,
                            (EnvironmentStackItem::ItemValue) (XInteger) Buf.Value.StringObjectPointer->Length});
            break;
        case XScript::EnvObject::ObjectKind::BytesObject:
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::Integer,
                            (EnvironmentStackItem::ItemValue) (XInteger) Buf.Value.BytesObjectPointer->Length});
            break;
        default:
            throw XScript::BytecodeInterpretError(L"RuntimeError: Expected a buffer");
    }

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void System_currentHeapSize(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    EnvironmentStackItem::ItemKind::Integer,
                    (EnvironmentStackItem::ItemValue) (XInteger) Interpreter->InterpreterEnvironment->Heap.HeapData.size()});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void System_getOptionalData(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    try {
        auto Data = Interpreter->InterpreterEnvironment->OptionalData.at(
                &GetStringObject(
                        *Interpreter,
                        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack())->Dest);
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {EnvironmentStackItem::ItemKind::HeapPointer,
                 (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                         {XScript::EnvObject::ObjectKind::StringObject,
                          (EnvObject::ObjectValue) CreateEnvStringObjectFromXString(Data)})});
    } catch (...) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"VMError",
                                                                 L"getOptionalData: data not exist."));
    }
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}

void System_setOptionalData(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Val = GetStringObject(
            *Interpreter,
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());
    auto Key = GetStringObject(
            *Interpreter,
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());

    Interpreter->InterpreterEnvironment->OptionalData.insert({&Key->Dest, &Val->Dest});
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) (XIndexType) {}});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}

void System_isEqual(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto R = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.HeapPointerVal;
    auto L = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.HeapPointerVal;

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Boolean, (EnvironmentStackItem::ItemValue) isEqual(Interpreter, L, R)});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}
