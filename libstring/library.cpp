#include <iostream>
#include <climits>

#include "library.h"


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {1, fromBuffer};
    Methods[XScript::Hash(L"fromInt")] = {1, fromInt};
    Methods[XScript::Hash(L"fromDeci")] = {1, fromDeci};
    Methods[XScript::Hash(L"fromBool")] = {1, fromBool};
    Methods[XScript::Hash(L"startsWith")] = {2, startsWith};
    Methods[XScript::Hash(L"endsWith")] = {2, endsWith};
    Methods[XScript::Hash(L"find")] = {2, find};
    Methods[XScript::Hash(L"substr")] = {2, substr};
    Methods[builtin_hash_code___instruction_add__] = {2, __instruction_add__};

    return {
            L"Jerry Chou",
            L"XScript 2 LibString",
            Methods};
}

XScript::EnvClassObject *CloneStringObject(XScript::BytecodeInterpreter *Interpreter) {
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

    XScript::EnvClassObject *Object = CloneStringObject(Interpreter);

    Object->Members[XScript::Hash(L"__buffer__")] = Item.Value.HeapPointerVal;
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
    XScript::EnvClassObject *Object = CloneStringObject(Interpreter);

    Object->Members[XScript::Hash(L"__buffer__")] =
            Interpreter->InterpreterEnvironment->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(
                                                  std::to_wstring(Item.Value.IntVal))}});

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
    XScript::EnvClassObject *Object = CloneStringObject(Interpreter);

    Object->Members[XScript::Hash(L"__buffer__")] =
            Interpreter->InterpreterEnvironment->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(
                                                  std::to_wstring(Item.Value.DeciVal))}});

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
    XScript::EnvClassObject *Object = CloneStringObject(Interpreter);

    Object->Members[XScript::Hash(L"__buffer__")] =
            Interpreter->InterpreterEnvironment->Heap.PushElement(
                    (XScript::EnvObject) {XScript::EnvObject::ObjectKind::StringObject,
                                          (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(
                                                  Item.Value.BoolVal ? L"True" : L"False")}});

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

    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            break;
    }

    XScript::XCharacter *p1 = &L->Dest, *p2 = &R->Dest;
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
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            break;
    }

    XScript::XCharacter *p1 = &L->Dest + L->Length - 1, *p2 = &R->Dest;
    for (; p1 != &L->Dest - 1 and *p2; --p1, ++p2) {
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

void find(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::XIndexType Start = 0;
    XScript::EnvironmentStackItem Right = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    if (Param.ParamsCount == 3) {
        Start = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    }
    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            break;
    }

    XScript::XInteger Res = static_cast<XScript::XInteger>((XScript::XString) {&L->Dest}.find(&R->Dest, Start));
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
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;

    End = End == INT_MAX ? L->Length : Start + End;

    XScript::XString Got;
    for (XScript::XIndexType i = Start; i < End && i < L->Length; i++) {
        Got += (&L->Dest)[i];
    }

    XScript::EnvClassObject *Result = XScript::NewEnvClassObject();
    *Result = *Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer;
    Result->Members[XScript::Hash(
            L"__buffer__")] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {XScript::EnvObject::ObjectKind::StringObject,
             (XScript::EnvObject::ObjectValue) {XScript::CreateEnvStringObjectFromXString(Got)}});

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
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;

    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];
    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[XScript::Hash(
                    L"__buffer__")]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            break;
    }

    auto Str = CloneStringObject(Interpreter);
    Str->Members[XScript::Hash(L"__buffer__")] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::StringObject,
                    (XScript::EnvObject::ObjectValue) XScript::MergeEnvStringObject(
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

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {static_cast<XScript::XIndexType>(0)});
}
