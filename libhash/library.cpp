#include <iostream>
#include <climits>

#include "library.h"
#include "../Share/Utils.hpp"


extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"create")] = {1, HashMap_create};
    Methods[XScript::Hash(L"insert")] = {3, HashMap_insert};
    Methods[XScript::Hash(L"__instruction_indexOf__")] = {3, HashMap___instruction_indexOf__};
    Methods[XScript::Hash(L"erase")] = {3, HashMap_erase};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"HashMap")] = {L"HashMap", Methods};

    return {
            L"Jerry Chou",
            L"XScript 2 LibString",
            Classes};
}

XScript::EnvClassObject *CloneHashMapObject(XScript::BytecodeInterpreter *Interpreter) {
    using namespace XScript;
    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From].Value.HeapPointerVal
    ].Value.ClassObjectPointer;
    XScript::EnvClassObject *New = XScript::NewEnvClassObject();
    *New = XScript::EnvClassObject{*This};

    XScript::EnvClassObject *Saver = XScript::NewEnvClassObject();
    New->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {XScript::EnvObject::ObjectKind::ClassObject, (EnvObject::ObjectValue) Saver});

    return New;
}

void HashMap_create(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    PushClassObjectStructure(Interpreter, CloneHashMapObject(Interpreter));
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}

void HashMap_insert(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XScript::EnvClassObject *Buffer = Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From].Value.HeapPointerVal
    ].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.ClassObjectPointer;
    EnvironmentStackItem Object = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    EnvironmentStackItem Key = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::XHeapIndexType Idx;
    switch (Object.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer:
            Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Integer,
                            (XScript::EnvObject::ObjectValue) {Object.Value.IntVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Decimal:
            Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Decimal,
                            (XScript::EnvObject::ObjectValue) {Object.Value.DeciVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Boolean:
            Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Boolean,
                            (XScript::EnvObject::ObjectValue) {Object.Value.BoolVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::HeapPointer:
            Idx = Object.Value.HeapPointerVal;
            break;
        default:
            throw XScript::BytecodeInterpretError(L"LibHash: Expected an available value.");
    }
    switch (Key.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer: {
            Buffer->Members.insert({static_cast<XHeapIndexType>(Key.Value.IntVal), Idx});
            break;
        }
        case XScript::EnvironmentStackItem::ItemKind::HeapPointer: {
            auto Val = Interpreter->InterpreterEnvironment->Heap.HeapData[Key.Value.HeapPointerVal];
            switch (Val.Kind) {
                case XScript::EnvObject::ObjectKind::StringObject: {
                    Buffer->Members.insert(
                            {static_cast<XHeapIndexType>(Hash(CovertToXString(Val.Value.StringObjectPointer))), Idx});
                    break;
                }
                case XScript::EnvObject::ObjectKind::BytesObject: {
                    Buffer->Members.insert(
                            {static_cast<XHeapIndexType>(HashBytes(CovertToXBytes(Val.Value.BytesObjectPointer))),
                             Idx});
                    break;
                }
                default: {
                    throw XScript::BytecodeInterpretError(L"LibHash: Expected an available key.");
                }
            }
            break;
        }
        default: {
            throw XScript::BytecodeInterpretError(L"LibHash: Expected an available key.");
        }
    }
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}

void HashMap___instruction_indexOf__(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XScript::EnvClassObject *Buffer = Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From].Value.HeapPointerVal
    ].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.ClassObjectPointer;
    EnvironmentStackItem Key = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XHeapIndexType Idx;
    switch (Key.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer: {
            Idx = Buffer->Members.at(static_cast<XHeapIndexType>(Key.Value.IntVal));
            break;
        }
        case XScript::EnvironmentStackItem::ItemKind::HeapPointer: {
            auto Val = Interpreter->InterpreterEnvironment->Heap.HeapData[Key.Value.HeapPointerVal];
            switch (Val.Kind) {
                case XScript::EnvObject::ObjectKind::StringObject: {
                    Idx = Buffer->Members.at(
                            static_cast<XHeapIndexType>(Hash(CovertToXString(Val.Value.StringObjectPointer))));
                    break;
                }
                case XScript::EnvObject::ObjectKind::BytesObject: {
                    Idx = Buffer->Members.at(
                            static_cast<XHeapIndexType>(HashBytes(CovertToXBytes(Val.Value.BytesObjectPointer))));
                    break;
                }
                default: {
                    throw XScript::BytecodeInterpretError(L"LibHash: Expected an available key.");
                }
            }
            break;
        }
        default: {
            throw XScript::BytecodeInterpretError(L"LibHash: Expected an available key.");
        }
    }
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::HeapPointer, (EnvironmentStackItem::ItemValue) Idx});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}

void HashMap_erase(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XScript::EnvClassObject *Buffer = Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From].Value.HeapPointerVal
    ].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.ClassObjectPointer;
    EnvironmentStackItem Key = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    switch (Key.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer: {
            Buffer->Members.erase(static_cast<XHeapIndexType>(Key.Value.IntVal));
            break;
        }
        case XScript::EnvironmentStackItem::ItemKind::HeapPointer: {
            auto Val = Interpreter->InterpreterEnvironment->Heap.HeapData[Key.Value.HeapPointerVal];
            switch (Val.Kind) {
                case XScript::EnvObject::ObjectKind::StringObject: {
                    Buffer->Members.erase(
                            static_cast<XHeapIndexType>(Hash(CovertToXString(Val.Value.StringObjectPointer))));
                    break;
                }
                case XScript::EnvObject::ObjectKind::BytesObject: {
                    Buffer->Members.erase(
                            static_cast<XHeapIndexType>(HashBytes(CovertToXBytes(Val.Value.BytesObjectPointer))));
                    break;
                }
                default: {
                    throw XScript::BytecodeInterpretError(L"LibHash: Expected an available key.");
                }
            }
            break;
        }
        default: {
            throw XScript::BytecodeInterpretError(L"LibHash: Expected an available key.");
        }
    }
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) (XIndexType) {});
}
