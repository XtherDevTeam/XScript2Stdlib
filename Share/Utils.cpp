//
// Created by Jerry Chou on 2022/8/6.
//

#include <cmath>
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
                return Interpreter.InterpreterEnvironment->Heap.HeapData[First.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
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

XScript::XHeapIndexType CloneObject(XScript::BytecodeInterpreter *Interpreter, XScript::XHeapIndexType Target) {
    using namespace XScript;
    auto I = Interpreter->InterpreterEnvironment->Heap.HeapData[Target];
    switch (I.Kind) {
        case XScript::EnvObject::ObjectKind::ArrayObject: {
            EnvArrayObject *Arr = NewEnvArrayObject(I.Value.ArrayObjectPointer->Length());
            XIndexType N = 0;
            for (auto &item : Arr->Elements) {
                Arr->Elements[N++] = CloneObject(Interpreter, item);
            }
            return Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {XScript::EnvObject::ObjectKind::ArrayObject, (EnvObject::ObjectValue) Arr});
        }
        case XScript::EnvObject::ObjectKind::StringObject: {
            EnvStringObject *Str = CreateEnvStringObject(I.Value.StringObjectPointer->Length);
            for (XIndexType Idx = 0;Idx < I.Value.StringObjectPointer->Length;Idx++) {
                (&Str->Dest)[Idx] = (&I.Value.StringObjectPointer->Dest)[Idx];
            }
            return Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {XScript::EnvObject::ObjectKind::StringObject, (EnvObject::ObjectValue) Str});
        }
        case XScript::EnvObject::ObjectKind::ClassObject: {
            EnvClassObject *Obj = NewEnvClassObject();
            XIndexType N = 0;
            Obj->Self = I.Value.ClassObjectPointer->Self;
            Obj->Parent = I.Value.ClassObjectPointer->Parent;
            for (auto &item : I.Value.ClassObjectPointer->Members) {
                Obj->Members[item.first] = CloneObject(Interpreter, item.second);
            }
            return Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {XScript::EnvObject::ObjectKind::ArrayObject, (EnvObject::ObjectValue) Obj});
        }
        case XScript::EnvObject::ObjectKind::BytesObject: {
            EnvBytesObject *Str = CreateEnvBytesObject(I.Value.BytesObjectPointer->Length);
            for (XIndexType Idx = 0;Idx < I.Value.BytesObjectPointer->Length;Idx++) {
                (&Str->Dest)[Idx] = (&I.Value.BytesObjectPointer->Dest)[Idx];
            }
            return Interpreter->InterpreterEnvironment->Heap.PushElement(
                    {XScript::EnvObject::ObjectKind::BytesObject, (EnvObject::ObjectValue) Str});
        }
        default:
            return Interpreter->InterpreterEnvironment->Heap.PushElement(I);
    }
}

XScript::EnvBytesObject *
GetBytesObject(XScript::BytecodeInterpreter &Interpreter, const XScript::EnvironmentStackItem &Item) {
    if (Item.Kind == XScript::EnvironmentStackItem::ItemKind::HeapPointer) {
        auto &First = Interpreter.InterpreterEnvironment->Heap.HeapData[Item.Value.HeapPointerVal];
        switch (First.Kind) {
            case XScript::EnvObject::ObjectKind::StringObject:
                return First.Value.BytesObjectPointer;
            case XScript::EnvObject::ObjectKind::ClassObject:
                return Interpreter.InterpreterEnvironment->Heap.HeapData[First.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
            default:
                return nullptr;
        }
    }
    return nullptr;
}

bool isEqual(XScript::BytecodeInterpreter *Interpreter, XScript::XHeapIndexType A, XScript::XHeapIndexType B) {
    auto &I = Interpreter->InterpreterEnvironment->Heap.HeapData[A];
    auto &J = Interpreter->InterpreterEnvironment->Heap.HeapData[B];
    if (I.Kind == J.Kind) {
        switch (I.Kind) {
            case XScript::EnvObject::ObjectKind::ClassObject: {
                if (I.Value.ClassObjectPointer->Parent != J.Value.ClassObjectPointer->Parent)
                    return false;
                if (I.Value.ClassObjectPointer->Self != J.Value.ClassObjectPointer->Self)
                    return false;
                if (I.Value.ClassObjectPointer->Members.size() != J.Value.ClassObjectPointer->Members.size())
                    return false;
                for (auto i = I.Value.ClassObjectPointer->Members.begin(), j = J.Value.ClassObjectPointer->Members.begin();
                     i != I.Value.ClassObjectPointer->Members.end(); i++, j++) {
                    if (i->first != j->first or !isEqual(Interpreter, i->second, j->second))
                        return false;
                }
                return true;
            }
            case XScript::EnvObject::ObjectKind::ArrayObject: {
                if (I.Value.ArrayObjectPointer->Elements.size() != J.Value.ArrayObjectPointer->Elements.size())
                    return false;
                for (auto i = I.Value.ArrayObjectPointer->Elements.begin(), j = J.Value.ArrayObjectPointer->Elements.begin();
                     i != I.Value.ArrayObjectPointer->Elements.end(); i++, j++) {
                    if (!isEqual(Interpreter, *i, *j))
                        return false;
                }
                return true;
            }
            case XScript::EnvObject::ObjectKind::StringObject: {
                if (I.Value.StringObjectPointer->Length != J.Value.StringObjectPointer->Length)
                    return false;
                for (XScript::XCharacter *i = &I.Value.StringObjectPointer->Dest, *j = &J.Value.StringObjectPointer->Dest;
                     *i; i++, j++) {
                    if (*i != *j)
                        return false;

                }
                return true;
            }
            case XScript::EnvObject::ObjectKind::BytesObject: {
                if (I.Value.BytesObjectPointer->Length != J.Value.BytesObjectPointer->Length)
                    return false;
                for (char *i = &I.Value.BytesObjectPointer->Dest, *j = &J.Value.BytesObjectPointer->Dest;
                     *i; i++, j++) {
                    if (*i != *j)
                        return false;

                }
                return true;
            }
            case XScript::EnvObject::ObjectKind::Integer:
                return I.Value.IntegerValue == J.Value.IntegerValue;
            case XScript::EnvObject::ObjectKind::Decimal:
                return I.Value.DecimalValue == J.Value.DecimalValue;
            case XScript::EnvObject::ObjectKind::Boolean:
                return I.Value.BooleanValue == J.Value.BooleanValue;
            case XScript::EnvObject::ObjectKind::Index:
                return I.Value.IndexValue == J.Value.IndexValue;
            case XScript::EnvObject::ObjectKind::FunctionPointer:
                return I.Value.FunctionPointerValue == J.Value.FunctionPointerValue;
            case XScript::EnvObject::ObjectKind::NativeMethodPointer:
                return I.Value.NativeMethodPointerValue == J.Value.NativeMethodPointerValue;
            case XScript::EnvObject::ObjectKind::ClosurePointer:
                return I.Value.ClosurePointer == J.Value.ClosurePointer;
        }
    } else {
        return false;
    }
}