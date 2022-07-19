#include <iostream>

#include "library.h"


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {0, fromBuffer};
    Methods[XScript::Hash(L"fromInt")] = {0, fromInt};
    Methods[XScript::Hash(L"fromDeci")] = {0, fromDeci};
    Methods[XScript::Hash(L"fromBool")] = {0, fromBool};

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

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer)};
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

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer)};
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

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer)};
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

    XScript::BytecodeInterpreter Interpreter{*static_cast<XScript::Environment *>(Param.VMPointer)};
    Interpreter.InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}
