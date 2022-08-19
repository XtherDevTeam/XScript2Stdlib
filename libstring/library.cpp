#include <iostream>
#include <climits>
#include <bitset>

#include "library.h"
#include "../Share/Utils.hpp"


FormatterTagInfo::FormatterTagInfo() : Kind(TagKind::Str), Base(TagBS::Dec), Precision(6) {

}

XScript::XCharacter *FormatterTagInfo::Parse(XScript::XCharacter *Str) {
    XScript::XString A;
    XScript::XString B;

    while (*Str != ' ') if (*Str == L'\0') { return nullptr; } else A += *(Str++);
    if (A == L"str") {
        Kind = TagKind::Str;
    } else if (A == L"int") {
        Kind = TagKind::Int;
    } else if (A == L"deci") {
        Kind = TagKind::Deci;
    } else if (A == L"bool") {
        Kind = TagKind::Bool;
    } else if (A == L"dol") {
        Kind = TagKind::Dol;
    } else {
        return nullptr;
    }
    if (Kind == TagKind::Int || Kind == TagKind::Deci) {
        A = L"";
        Str++;
        while (*Str != L'=') if (*Str == L'\0') { return nullptr; } else A += *(Str++);

        while (*Str != L'}') if (*Str == L'\0') { return nullptr; } else B += *(Str++);

        if (A == L"bs") {
            if (B == L"hex")
                Base = TagBS::Hex;
            else if (B == L"dec")
                Base = TagBS::Dec;
            else if (B == L"oct")
                Base = TagBS::Oct;
            else if (B == L"bin")
                Base = TagBS::Bin;
            else
                return nullptr;
        } else if (A == L"pre") {
            try {
                Precision = std::stol(B);
            } catch (...) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return Str;
}


extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"fromBuffer")] = {1, fromBuffer};
    Methods[XScript::Hash(L"fromBytes")] = {1, fromBytes};
    Methods[XScript::Hash(L"fromInt")] = {1, fromInt};
    Methods[XScript::Hash(L"fromDeci")] = {1, fromDeci};
    Methods[XScript::Hash(L"fromBool")] = {1, fromBool};
    Methods[XScript::Hash(L"startsWith")] = {2, startsWith};
    Methods[XScript::Hash(L"endsWith")] = {2, endsWith};
    Methods[XScript::Hash(L"find")] = {2, find};
    Methods[XScript::Hash(L"substr")] = {2, substr};
    Methods[XScript::Hash(L"toInt")] = {2, toInt};
    Methods[XScript::Hash(L"toBool")] = {2, toBool};
    Methods[XScript::Hash(L"toBytes")] = {2, toBytes};
    Methods[XScript::Hash(L"toDeci")] = {2, toDeci};
    Methods[XScript::Hash(L"formatter")] = {2, formatter};
    Methods[builtin_hash_code___instruction_add__] = {2, __instruction_add__};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"String")] = {L"String", Methods};

    return {
            L"Jerry Chou",
            L"XScript 2 LibString",
            Classes};
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

    Object->Members[builtin_hashcode___buffer__] = Item.Value.HeapPointerVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::HeapPointer, (XScript::EnvironmentStackItem::ItemValue) {
                    Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {XScript::EnvObject::ObjectKind::ClassObject,
                             (XScript::EnvObject::ObjectValue) {Object}})
            }});

    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) {(XScript::XIndexType) {0}});
}

void fromBytes(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    Interpreter->GC->Start();

    XScript::EnvironmentStackItem Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();

    XScript::EnvClassObject *Object = CloneStringObject(Interpreter);

    Object->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {XScript::EnvObject::ObjectKind::StringObject,
             (XScript::EnvObject::ObjectValue) XScript::CreateEnvStringObjectFromXString(XScript::string2wstring(
                     CovertToXBytes(
                             Interpreter->InterpreterEnvironment->Heap.HeapData[Item.Value.HeapPointerVal].Value.BytesObjectPointer)))}
    );
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

    Object->Members[builtin_hashcode___buffer__] =
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

    Object->Members[builtin_hashcode___buffer__] =
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

    Object->Members[builtin_hashcode___buffer__] =
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
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            throw XScript::BytecodeInterpretError(L"LibString: Expected a string object");
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
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
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

    XScript::XCharacter *p1 = &L->Dest + L->Length - R->Length, *p2 = &R->Dest;
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
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];

    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            throw XScript::BytecodeInterpretError(L"LibString: Expected a string object");
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
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;

    End = End == INT_MAX ? L->Length : Start + End;

    XScript::XString Got;
    for (XScript::XIndexType i = Start; i < End && i < L->Length; i++) {
        Got += (&L->Dest)[i];
    }

    XScript::EnvClassObject *Result = XScript::NewEnvClassObject();
    *Result = *Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer;
    Result->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
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
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;

    XScript::EnvObject &ToCompare =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Right.Value.HeapPointerVal];
    XScript::EnvStringObject *R;

    switch (ToCompare.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject:
            R = Interpreter->InterpreterEnvironment->Heap.HeapData[ToCompare.Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;
            break;
        case XScript::EnvObject::ObjectKind::StringObject:
            R = ToCompare.Value.StringObjectPointer;
            break;
        default:
            throw XScript::BytecodeInterpretError(L"LibString: Expected a string object");
    }

    auto Str = CloneStringObject(Interpreter);
    Str->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
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

    Interpreter->InstructionFuncReturn(
            (XScript::BytecodeStructure::InstructionParam) {static_cast<XScript::XIndexType>(0)});
}

void toInt(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;

    try {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        XScript::EnvironmentStackItem::ItemKind::Integer,
                        (XScript::EnvironmentStackItem::ItemValue) static_cast<XScript::XInteger>(stol(
                                CovertToXString(L)))
                });
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    } catch (const std::exception &E) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"CovertError",
                                                                 L"Cannot covert string to integer"));
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    }
}

void toDeci(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;

    try {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        XScript::EnvironmentStackItem::ItemKind::Decimal,
                        (XScript::EnvironmentStackItem::ItemValue) static_cast<XScript::XDecimal>(stof(
                                CovertToXString(L)))
                });
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    } catch (const std::exception &E) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"CovertError",
                                                                 L"Cannot covert string to integer"));
        Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
    }
}

void toBool(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    XScript::EnvironmentStackItem::ItemKind::Boolean,
                    (XScript::EnvironmentStackItem::ItemValue) (CovertToXString(L) == L"True")
            });
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
}

void toBytes(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvironmentStackItem Left = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From];
    XScript::EnvStringObject *L =
            Interpreter->InterpreterEnvironment->Heap.HeapData[Interpreter->InterpreterEnvironment->Heap.HeapData[Left.Value.HeapPointerVal].Value.ClassObjectPointer->Members[builtin_hashcode___buffer__]].Value.StringObjectPointer;

    XScript::XHeapIndexType IndexOfBytes = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::BytesObject,
                    (XScript::EnvObject::ObjectValue) XScript::CreateEnvBytesObjectFromXBytes(
                            XScript::wstring2string(CovertToXString(L)))});

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    XScript::EnvironmentStackItem::ItemKind::HeapPointer,
                    (XScript::EnvironmentStackItem::ItemValue) IndexOfBytes
            });
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
}

void length(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    auto Str =
            GetStringObject(*Interpreter,
                            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {XScript::EnvironmentStackItem::ItemKind::Integer, (XScript::EnvironmentStackItem::ItemValue) Str->Length});
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
}

void formatter(XScript::ParamToMethod Param) {
    using XScript::BytecodeInterpreter;
    auto *Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::XIndexType Cur =
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From + 1;
    XScript::XString Res;
    auto Format =
            GetStringObject(*Interpreter,
                            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(Cur - 1));

    FormatterTagInfo CurTag;
    for (XScript::XCharacter *Ch = &Format->Dest; Ch < &Format->Dest + Format->Length; Ch++) {
        if (*Ch == L'$' && *(Ch + 1) == L'{') {
            Ch += 2;
            if (Ch < &Format->Dest + Format->Length) {
                CurTag = {};
                Ch = CurTag.Parse(Ch);

                if (Ch) {
                    switch (CurTag.Kind) {
                        case FormatterTagInfo::TagKind::Str: {
                            Res += &GetStringObject(*Interpreter,
                                                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                                            Cur++))->Dest;
                            break;
                        }
                        case FormatterTagInfo::TagKind::Int: {
                            std::wstringstream ss;
                            switch (CurTag.Base) {
                                case FormatterTagInfo::TagBS::Hex:
                                    ss << std::hex
                                       << Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                               Cur++).Value.IntVal;
                                    break;
                                case FormatterTagInfo::TagBS::Dec:
                                    ss << std::dec
                                       << Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                               Cur++).Value.IntVal;
                                    break;
                                case FormatterTagInfo::TagBS::Oct:
                                    ss << std::oct
                                       << Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                               Cur++).Value.IntVal;
                                    break;
                                case FormatterTagInfo::TagBS::Bin:
                                    ss << std::dec << std::bitset<8>(
                                            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                                    Cur++).Value.IntVal);
                                    break;
                            }
                            Res += ss.str();
                            break;
                        }
                        case FormatterTagInfo::TagKind::Deci: {
                            std::wstringstream ss;
                            ss << std::setprecision(static_cast<int>(CurTag.Precision))
                               << Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                       Cur++).Value.DeciVal;
                            Res += ss.str();
                            break;
                        }
                        case FormatterTagInfo::TagKind::Bool: {
                            Res += Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.GetValueFromStack(
                                    Cur++).Value.BoolVal ? L"True" : L"False";
                            break;
                        }
                        case FormatterTagInfo::TagKind::Dol: {
                            Res += L"$";
                            break;
                        }
                    }
                    if (*Ch != L'}') {
                        PushClassObjectStructure(Interpreter,
                                                 ConstructInternalErrorStructure(Interpreter, L"ParseError",
                                                                                 L"Expected a `}`"));
                        Interpreter->InstructionFuncReturn(
                                (XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
                        return;
                    }
                } else {
                    PushClassObjectStructure(Interpreter, ConstructInternalErrorStructure(Interpreter, L"ParseError",
                                                                                          L"Expected a correct tag"));
                    Interpreter->InstructionFuncReturn(
                            (XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
                    return;
                }
            }
        } else {
            Res += *Ch;
        }
    }

    auto Str = CloneStringObject(Interpreter);
    Str->Members[builtin_hashcode___buffer__] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::StringObject,
                    (XScript::EnvObject::ObjectValue) XScript::CreateEnvStringObjectFromXString(Res)});
    PushClassObjectStructure(Interpreter, Str);
    Interpreter->InstructionFuncReturn((XScript::BytecodeStructure::InstructionParam) (XScript::XInteger) {});
}
