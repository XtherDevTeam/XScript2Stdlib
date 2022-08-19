#include <iostream>
#include <thread>

#include "../Share/Utils.hpp"
#include "library.h"

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {2, fromBuffer};
    Methods[XScript::Hash(L"push_back")] = {2, push_back};
    Methods[XScript::Hash(L"pop_back")] = {1, pop_back};
    Methods[XScript::Hash(L"removeIndex")] = {2, removeIndex};
    Methods[XScript::Hash(L"resize")] = {2, resize};
    Methods[XScript::Hash(L"create")] = {1, create};
    Methods[XScript::Hash(L"length")] = {1, length};
    Methods[builtin_hash_code___instruction_indexOf__] = {1, __instruction_indexOf__};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"Array")] = {L"Array", Methods};

    return {
            L"Jerry Chou",
            L"XScript 2 LibArray",
            Classes};
}

XScript::EnvClassObject *CloneArrayObject(XScript::BytecodeInterpreter *Interpreter) {
    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;
    XScript::EnvClassObject *New = XScript::NewEnvClassObject();
    *New = XScript::EnvClassObject{*This};
    New->Members[builtin_hashcode___buffer__] = {Interpreter->InterpreterEnvironment->Heap.PushElement(
            {XScript::EnvObject::ObjectKind::ArrayObject,
             (XScript::EnvObject::ObjectValue) {
                     XScript::NewEnvArrayObject(0)}})};
    return New;
}

void fromBuffer(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XScript::EnvironmentStackItem Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneArrayObject(Interpreter);

    Object->Members[builtin_hashcode___buffer__] = Item.Value.HeapPointerVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void push_back(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *Object = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;
    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Object->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;

    XScript::EnvironmentStackItem ToPush = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::XHeapIndexType Idx;
    switch (ToPush.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer:
            Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Integer,
                            (XScript::EnvObject::ObjectValue) {ToPush.Value.IntVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Decimal:
            Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Decimal,
                            (XScript::EnvObject::ObjectValue) {ToPush.Value.DeciVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Boolean:
            Idx = Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Boolean,
                            (XScript::EnvObject::ObjectValue) {ToPush.Value.BoolVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::HeapPointer:
            Idx = ToPush.Value.HeapPointerVal;
            break;
        default:
            throw XScript::BytecodeInterpretError(L"LibArray: Expected an available value.");
    }
    Array->Elements.push_back(Idx);

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void pop_back(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *Object = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Object->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;

    Array->Elements.pop_back();

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void __instruction_indexOf__(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *Object = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Object->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;
    XScript::XInteger Index = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (XScript::EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::HeapPointer,
                    (XScript::EnvironmentStackItem::ItemValue) {Array->Elements[Index]}
            });
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void removeIndex(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *Object = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Object->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;

    XScript::XInteger Index = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    Array->Elements.erase(Array->Elements.begin() + Index);

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void resize(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    Interpreter->GC->Start();

    XScript::EnvClassObject *Object = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Object->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;

    XScript::XInteger Index = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    Array->Elements.resize(Index);
    for (auto &I: Array->Elements) {
        I = Interpreter->InterpreterEnvironment->Heap.PushElement({
                                                                          XScript::EnvObject::ObjectKind::Integer,
                                                                          (XScript::EnvObject::ObjectValue) {
                                                                                  (XScript::XInteger) {}}
                                                                  });
    }
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void create(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *Obj = CloneArrayObject(Interpreter);
    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Obj->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;
    Array->Elements = {};
    XScript::XHeapIndexType Idx = Interpreter->InterpreterEnvironment->Heap.PushElement({
                                                                                                XScript::EnvObject::ObjectKind::ClassObject,
                                                                                                (XScript::EnvObject::ObjectValue) {
                                                                                                        Obj}
                                                                                        });
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (XScript::EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::HeapPointer,
                    (XScript::EnvironmentStackItem::ItemValue) {Idx}
            });

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void length(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XScript::EnvClassObject *Object = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto *Array =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Object->Members[builtin_hashcode___buffer__]].Value.ArrayObjectPointer;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (XScript::EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::Integer,
                    (XScript::EnvironmentStackItem::ItemValue) {Array->Length()}
            });
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}
