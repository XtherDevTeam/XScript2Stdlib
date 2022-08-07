//
// Created by Jerry Chou on 2022/8/6.
//

#include "Utils.hpp"

XScript::EnvClassObject *
ConstructInternalErrorStructure(XScript::BytecodeInterpreter *Interpreter, const XScript::XString &ErrorName,
                                const XScript::XString &ErrorDescription) {
    using namespace XScript;
    EnvClassObject *Object = NewEnvClassObject();
    Object->Self = {Hash(L"Error"), Hash(L"Error")};
    Object->Members[Hash(L"errorName")] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::StringObject,
                    (EnvObject::ObjectValue) CreateEnvStringObjectFromXString(ErrorName)
            });
    Object->Members[Hash(L"errorDescription")] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::StringObject,
                    (EnvObject::ObjectValue) CreateEnvStringObjectFromXString(ErrorDescription)
            });

    return Object;
}


XScript::EnvStringObject *
GetStringObject(XScript::BytecodeInterpreter &Interpreter, const XScript::EnvironmentStackItem &Item) {
    if (Item.Kind == XScript::EnvironmentStackItem::ItemKind::HeapPointer) {
        auto &First = Interpreter.InterpreterEnvironment->Heap.HeapData[Item.Value.HeapPointerVal];
        switch (First.Kind) {
            case XScript::EnvObject::ObjectKind::StringObject:
                return First.Value.StringObjectPointer;
            case XScript::EnvObject::ObjectKind::ClassObject:
                return Interpreter.InterpreterEnvironment->Heap.HeapData[First.Value.ClassObjectPointer->Members[XScript::Hash(
                        L"__buffer__")]].Value.StringObjectPointer;
            default:
                return nullptr;
        }
    }
    return nullptr;
}

void PushClassObjectStructure(XScript::BytecodeInterpreter *Interpreter, XScript::EnvClassObject *Object) {
    using namespace XScript;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    EnvironmentStackItem::ItemKind::HeapPointer,
                    (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {EnvObject::ObjectKind::ClassObject,
                             (EnvObject::ObjectValue) Object
                            })
            });
}
