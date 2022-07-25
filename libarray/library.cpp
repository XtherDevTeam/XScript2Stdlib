#include <iostream>

#include "library.h"

extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {2, fromBuffer};
    Methods[XScript::Hash(L"push_back")] = {2, push_back};
    Methods[XScript::Hash(L"pop_back")] = {1, pop_back};
    Methods[XScript::Hash(L"removeIndex")] = {2, removeIndex};
    Methods[XScript::Hash(L"resize")] = {2, resize};
    Methods[XScript::Hash(L"create")] = {1, create};
    Methods[XScript::Hash(L"__instruction_indexOf__")] = {1, __instruction_indexOf__};
    return {
            L"Jerry Chou",
            L"XScript 2 LibArray",
            Methods};
}

XScript::EnvClassObject *CloneArrayObject(XScript::Environment *Env) {
    XScript::EnvClassObject *This = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    XScript::EnvClassObject *New = XScript::NewEnvClassObject();
    *New = *This;
    New->Members[XScript::Hash(L"__buffer__")] = {Env->Heap.PushElement({XScript::EnvObject::ObjectKind::StringObject,
                                                                         (XScript::EnvObject::ObjectValue) {
                                                                                 XScript::NewEnvArrayObject(1)}})};
    return New;
}

void fromBuffer(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Item = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneArrayObject(static_cast<XScript::Environment *>(Param.VMPointer));

    Object->Members[XScript::Hash(L"__buffer__")] = Item.Value.HeapPointerVal;
    static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void push_back(XScript::ParamToMethod Param) {
    auto *Env = static_cast<XScript::Environment *>(Param.VMPointer);
    XScript::EnvClassObject *Object = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    auto *Array =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Object->Members[XScript::Hash(
                    L"__buffer__")]].Value.ArrayObjectPointer;

    XScript::EnvironmentStackItem ToPush = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::XHeapIndexType Idx;
    switch (ToPush.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer:
            Idx = static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Integer,
                            (XScript::EnvObject::ObjectValue) {ToPush.Value.IntVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Decimal:
            Idx = static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                    {
                            XScript::EnvObject::ObjectKind::Decimal,
                            (XScript::EnvObject::ObjectValue) {ToPush.Value.DeciVal}});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Boolean:
            Idx = static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
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
    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};

    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void pop_back(XScript::ParamToMethod Param) {
    auto *Env = static_cast<XScript::Environment *>(Param.VMPointer);
    XScript::EnvClassObject *Object = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    auto *Array =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Object->Members[XScript::Hash(
                    L"__buffer__")]].Value.ArrayObjectPointer;
    Array->Elements.pop_back();
    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};

    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void __instruction_indexOf__(XScript::ParamToMethod Param) {
    auto *Env = static_cast<XScript::Environment *>(Param.VMPointer);
    XScript::EnvClassObject *Object = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    auto *Array =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Object->Members[XScript::Hash(
                    L"__buffer__")]].Value.ArrayObjectPointer;
    XScript::XInteger Index = Env->Stack.PopValueFromStack().Value.IntVal;
    Env->Stack.PushValueToStack((XScript::EnvironmentStackItem) {
            XScript::EnvironmentStackItem::ItemKind::HeapPointer,
            (XScript::EnvironmentStackItem::ItemValue) {Array->Elements[Index]}
    });

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void removeIndex(XScript::ParamToMethod Param) {
    auto *Env = static_cast<XScript::Environment *>(Param.VMPointer);
    XScript::EnvClassObject *Object = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    auto *Array =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Object->Members[XScript::Hash(
                    L"__buffer__")]].Value.ArrayObjectPointer;
    XScript::XInteger Index = Env->Stack.PopValueFromStack().Value.IntVal;
    Array->Elements.erase(Array->Elements.begin() + Index);
    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void resize(XScript::ParamToMethod Param) {
    auto *Env = static_cast<XScript::Environment *>(Param.VMPointer);
    XScript::EnvClassObject *Object = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    auto *Array =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Object->Members[XScript::Hash(
                    L"__buffer__")]].Value.ArrayObjectPointer;
    XScript::XInteger Index = Env->Stack.PopValueFromStack().Value.IntVal;
    Array->Elements.resize(Index);
    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}

void create(XScript::ParamToMethod Param) {
    auto *Env = static_cast<XScript::Environment *>(Param.VMPointer);
    XScript::EnvClassObject *Obj = CloneArrayObject(Env);
    auto *Array =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Obj->Members[XScript::Hash(
                    L"__buffer__")]].Value.ArrayObjectPointer;
    Array->Elements = {};
    XScript::XHeapIndexType Idx = Env->Heap.PushElement({
        XScript::EnvObject::ObjectKind::ClassObject,
        (XScript::EnvObject::ObjectValue) {Obj}
    });
    Env->Stack.PushValueToStack((XScript::EnvironmentStackItem) {
        XScript::EnvironmentStackItem::ItemKind::HeapPointer,
        (XScript::EnvironmentStackItem::ItemValue) {Idx}
    });
    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer),
                                             *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XInteger) {}});
}
