#include <iostream>
#include <climits>

#include "library.h"
#include "../Share/Utils.hpp"


extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {1, fromBuffer};
    Methods[XScript::Hash(L"fromInt")] = {1, fromInt};
    Methods[XScript::Hash(L"fromDeci")] = {1, fromDeci};
    Methods[XScript::Hash(L"fromBool")] = {1, fromBool};
    Methods[XScript::Hash(L"startsWith")] = {2, startsWith};
    Methods[XScript::Hash(L"endsWith")] = {2, endsWith};
    Methods[XScript::Hash(L"find")] = {2, find};
    Methods[XScript::Hash(L"substr")] = {2, substr};
    Methods[XScript::Hash(L"toInt")] = {2, toInt};
    Methods[XScript::Hash(L"toBool")] = {2, toBool};
    Methods[XScript::Hash(L"toDeci")] = {2, toDeci};
    Methods[builtin_hash_code___instruction_add__] = {2, __instruction_add__};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"Bytes")] = {L"Bytes", Methods};

    return {
            L"Jerry Chou",
            L"XScript 2 LibBytes",
            Classes};
}

XScript::EnvClassObject *CloneBytesObject(XScript::BytecodeInterpreter *Interpreter) {
    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From].Value.HeapPointerVal
    ].Value.ClassObjectPointer;
    XScript::EnvClassObject *New = XScript::NewEnvClassObject();
    *New = XScript::EnvClassObject{*This};

    return New;
}

void fromBuffer(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    Interpreter->GC->Start();

    XScript::EnvironmentStackItem Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();

    XScript::EnvClassObject *Object = CloneBytesObject(Interpreter);

    Object->Members[builtin_hashcode___buffer__] = Item.Value.HeapPointerVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromInt(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    Interpreter->GC->Start();

    XScript::EnvironmentStackItem Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneBytesObject(Interpreter);

    Object->Members[builtin_hashcode___buffer__] =
            Interpreter->InterpreterEnvironment->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::BytesObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvBytesObjectFromXBytes(
                                                  std::to_string(Item.Value.IntVal))}});

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromDeci(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->GC->Start();

    XScript::EnvironmentStackItem Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneBytesObject(Interpreter);

    Object->Members[builtin_hashcode___buffer__] =
            Interpreter->InterpreterEnvironment->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::BytesObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvBytesObjectFromXBytes(
                                                  std::to_string(Item.Value.DeciVal))}});

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromBool(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->GC->Start();
    XScript::EnvironmentStackItem Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvClassObject *Object = CloneBytesObject(Interpreter);

    Object->Members[builtin_hashcode___buffer__] =
            Interpreter->InterpreterEnvironment->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::BytesObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvBytesObjectFromXBytes(
                                                  Item.Value.BoolVal ? "True" : "False")}});

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void startsWith(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Right = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];

    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvBytesObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::BytesObject:
            R = ToCompare.Value.BytesObjectPointer;
            break;
        default:
            break;
    }

    char *p1 = &L->Dest, *p2 = &R->Dest;
    for (; *p1 and *p2; ++p1, ++p2) {
        if (*p1 != *p2) {
            Interpreter->InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {false});
            break;
        }
    }

    if (*p2) {
        Interpreter->InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {false});
    } else {
        Interpreter->InstructionStackPushBoolean((XScript::BytecodeStructure::InstructionParam) {true});
    }

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {}});
}

void endsWith(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Right = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvBytesObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::BytesObject:
            R = ToCompare.Value.BytesObjectPointer;
            break;
        default:
            throw XScript::BytecodeInterpretError(L"RuntimeError: Expected a string object for rvalue.");
    }

    if (L->Length < R->Length) {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {XScript::EnvironmentStackItem::ItemKind::Boolean, (XScript::EnvironmentStackItem::ItemValue) false});
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (bool) {});
        return;
    }

    char *p1 = &L->Dest + L->Length - R->Length, *p2 = &R->Dest;
    while (*p1 && *p2) {
        if (*p1 == *p2) {
            p1++;
            p2++;
            continue;
        } else {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {XScript::EnvironmentStackItem::ItemKind::Boolean,
                     (XScript::EnvironmentStackItem::ItemValue) false});
            Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (bool) {});
            return;
        }
    }

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::Boolean, (XScript::EnvironmentStackItem::ItemValue) true});
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (bool) {});
}

void find(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::XIndexType Start = 0;
    XScript::EnvironmentStackItem Right = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    if (Param.ParamsCount == 3) {
        Start = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    }
    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvBytesObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::BytesObject:
            R = ToCompare.Value.BytesObjectPointer;
            break;
        default:
            break;
    }

    XScript::XInteger Res = static_cast<XScript::XInteger>((XScript::XBytes) {&L->Dest}.find(&R->Dest, Start));
    Interpreter->InstructionStackPushInteger((XScript::BytecodeStructure::InstructionParam) {Res});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {}});
}

void substr(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->GC->Start();

    XScript::XIndexType Start = 0, End = INT_MAX;
    if (Param.ParamsCount == 2) {
        Start = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    } else if (Param.ParamsCount == 3) {
        End = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
        Start = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    }

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;

    End = End == INT_MAX ? L->Length : Start + End;

    XScript::XBytes Got;
    for (XScript::XIndexType i = Start; i < End && i < L->Length; i++) {
        Got += (&L->Dest)[i];
    }

    XScript::EnvClassObject *Result = XScript::NewEnvClassObject();
    *Result = *Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer;
    Result->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {XScript::EnvObject::ObjectKind::BytesObject,
             (XScript::EnvObject::ObjectValue) {XScript::CreateEnvBytesObjectFromXBytes(Got)}});

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (XScript::EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::HeapPointer,
                    (XScript::EnvironmentStackItem::ItemValue) {
                            Interpreter->InterpreterEnvironment->Heap.PushElement(
                                    {XScript::EnvObject::ObjectKind::ClassObject,
                                     (XScript::EnvObject::ObjectValue) {Result}})}
            });

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {}});
}

void __instruction_add__(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->GC->Start();

    XScript::EnvironmentStackItem Right = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;

    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];
    XScript::EnvBytesObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::BytesObject:
            R = ToCompare.Value.BytesObjectPointer;
            break;
        default:
            break;
    }

    auto Str = CloneBytesObject(Interpreter);
    Str->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::BytesObject,
                    (XScript::EnvObject::ObjectValue) XScript::MergeEnvBytesObject(
                            L,
                            R)
            });

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    XScript::EnvironmentStackItem::ItemKind::HeapPointer,
                    (XScript::EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {
                                    XScript::EnvObject::ObjectKind::ClassObject,
                                    (XScript::EnvObject::ObjectValue) Str
                            })
            });

    Interpreter->InstructionFuncReturn(
            (XScript::BytecodeStructure::InstructionParam) {static_cast<XScript::XIndexType>(0)});
}

void toInt(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;

    try {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        XScript::EnvironmentStackItem::ItemKind::Integer,
                        (XScript::EnvironmentStackItem::ItemValue) static_cast<XScript::XInteger>(stol(CovertToXBytes(L)))
                });
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    } catch (const std::exception &E) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"CovertError",
                                                                 L"Cannot covert Bytes to integer"));
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    }
}

void toDeci(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;

    try {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        XScript::EnvironmentStackItem::ItemKind::Decimal,
                        (XScript::EnvironmentStackItem::ItemValue) static_cast<XScript::XDecimal>(stof(CovertToXBytes(L)))
                });
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    } catch (const std::exception &E) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"CovertError",
                                                                 L"Cannot covert Bytes to integer"));
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    }
}

void toBool(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvBytesObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.BytesObjectPointer;

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    XScript::EnvironmentStackItem::ItemKind::Boolean,
                    (XScript::EnvironmentStackItem::ItemValue) (CovertToXBytes(L) == "True")
            });
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
}

void length(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    auto Str =
            GetBytesObject(*Interpreter, Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::Integer, (XScript::EnvironmentStackItem::ItemValue) Str->Length});
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
}