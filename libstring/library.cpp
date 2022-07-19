#include <iostream>

#include "library.h"


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {1, fromBuffer};
    Methods[XScript::Hash(L"fromInt")] = {1, fromInt};
    Methods[XScript::Hash(L"fromDeci")] = {1, fromDeci};
    Methods[XScript::Hash(L"fromBool")] = {1, fromBool};
    Methods[XScript::Hash(L"startsWith")] = {1, startsWith};
    Methods[XScript::Hash(L"endsWith")] = {1, endsWith};

    return {
            L"Jerry Chou",
            L"XScript 2 LibString",
            Methods};
}

XScript::EnvClassObject *CloneStringObject(XScript::Environment *Env) {
    XScript::EnvClassObject *This = Env->Heap.HeapData[Env->Stack.Elements[Env->Stack.FramesInformation.back().From].Value.HeapPointerVal].Value.ClassObjectPointer;
    XScript::EnvClassObject *New = XScript::NewEnvClassObject();
    *New = *This;
    New->Members[XScript::Hash(L"__buffer__")] = {Env->Heap.PushElement({XScript::EnvObject::ObjectKind::StringObject,
                                                                         (XScript::EnvObject::ObjectValue) {
                                                                                 XScript::CreateEnvStringObject(1)}})};
    return New;
}

void fromBuffer(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Item = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneStringObject(static_cast<XScript::Environment *>(Param.VMPointer));

    Object->Members[XScript::Hash(L"__buffer__")] = Item.Value.HeapPointerVal;
    static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer), *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromInt(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Item = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneStringObject(static_cast<XScript::Environment *>(Param.VMPointer));

    Object->Members[XScript::Hash(L"__buffer__")] =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(
                                                  std::to_wstring(Item.Value.IntVal))}});

    static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer), *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromDeci(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Item = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneStringObject(static_cast<XScript::Environment *>(Param.VMPointer));

    Object->Members[XScript::Hash(L"__buffer__")] =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(
                                                  std::to_wstring(Item.Value.DeciVal))}});

    static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer), *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromBool(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Item = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneStringObject(static_cast<XScript::Environment *>(Param.VMPointer));

    Object->Members[XScript::Hash(L"__buffer__")] =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(
                                                  Item.Value.BoolVal ? L"True" : L"False")}});

    static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    static_cast<XScript::Environment *>(Param.VMPointer)->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer), *static_cast<XScript::GarbageCollection *>(Param.VMGC)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void startsWith(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Right = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvironmentStackItem Left = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.Elements[static_cast<XScript::Environment *>(Param.VMPointer)->Stack.FramesInformation.back().From];

    XScript::EnvStringObject *L =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            break;
    }

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer), *static_cast<XScript::GarbageCollection *>(Param.VMGC)};

    XScript::XCharacter *p1 = &L->Dest, *p2 = &R->Dest;
    for (; *p1 and *p2; ++p1, ++p2) {
        if (*p1 != *p2) {
            Interpreter.InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {false});
            break;
        }
    }

    if (*p2) {
        Interpreter.InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {false});
    } else {
        Interpreter.InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {true});
    }

    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {}});
}

void endsWith(XScript::ParamToMethod Param) {
    XScript::EnvironmentStackItem Right = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.PopValueFromStack();
    XScript::EnvironmentStackItem Left = static_cast<XScript::Environment *>(Param.VMPointer)->Stack.Elements[static_cast<XScript::Environment *>(Param.VMPointer)->Stack.FramesInformation.back().From];
    XScript::EnvStringObject *L =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = static_cast<XScript::Environment *>(Param.VMPointer)->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            break;
    }

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer), *static_cast<XScript::GarbageCollection *>(Param.VMGC)};

    XScript::XCharacter *p1 = &L->Dest + L->Length - 1, *p2 = &R->Dest;
    for (; p1 != &L->Dest - 1 and *p2; --p1, ++p2) {
        if (*p1 != *p2) {
            Interpreter.InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {false});
            break;
        }
    }

    if (*p2) {
        Interpreter.InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {false});
    } else {
        Interpreter.InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {true});
    }

    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {}});
}