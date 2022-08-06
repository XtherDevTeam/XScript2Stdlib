#include "library.h"
#include "../Share/Utils.hpp"

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"open")] = {3, File_open};
    Methods[XScript::Hash(L"close")] = {3, File_close};
    Methods[XScript::Hash(L"read")] = {3, File_read};
    Methods[XScript::Hash(L"write")] = {3, File_write};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"File")] = {L"File", Methods};

    Methods = {};
    Methods[XScript::Hash(L"exists")] = {3, FS_exists};
    Methods[XScript::Hash(L"isDirectory")] = {3, FS_isDirectory};
    Methods[XScript::Hash(L"listDirectory")] = {3, FS_listDirectory};
    Classes[XScript::Hash(L"FS")] = {L"FS", Methods};

    return {
            L"Jerry Chou",
            L"XScript 2 LibFS",
            Classes};
}


XScript::EnvClassObject *CloneFSObject(XScript::BytecodeInterpreter *Interpreter) {
    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    XScript::EnvClassObject *New = XScript::NewEnvClassObject();
    *New = XScript::EnvClassObject{*This};
    return New;
}

void File_open(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Temp = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvStringObject *Flags;

    Flags = GetStringObject(*Interpreter, Temp);
    if (Flags == nullptr) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"FileError", L"Cannot open file."));
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
        return;
    }

    Temp = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    XScript::EnvStringObject *Path;

    Path = GetStringObject(*Interpreter, Temp);
    if (Path == nullptr) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"FileError", L"Cannot open file."));
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
        return;
    }

    FILE *fp = fopen(wstring2string(CovertToXString(Path)).c_str(), wstring2string(CovertToXString(Flags)).c_str());

    if (fp == nullptr) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"FileError", L"Cannot open file."));
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
        return;
    }

    auto Object = CloneFSObject(Interpreter);

    Object->Members[Hash(L"__file__")] = Interpreter->InterpreterEnvironment->Heap.PushElement(
            {
                    XScript::EnvObject::ObjectKind::Index,
                    (XScript::EnvObject::ObjectValue) {(EnvBytesObject *) (fp)}
            });

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    EnvironmentStackItem::ItemKind::HeapPointer,
                    (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                            {EnvObject::ObjectKind::ClassObject, (EnvObject::ObjectValue) Object})
            });
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
}

void File_close(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto fp = (FILE *) (Interpreter->InterpreterEnvironment->Heap.HeapData[This->Members[Hash(
            L"__file__")]].Value.BytesObjectPointer);
    fclose(fp);

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
}

void File_read(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto fp = (FILE *) (Interpreter->InterpreterEnvironment->Heap.HeapData[This->Members[Hash(
            L"__file__")]].Value.BytesObjectPointer);
    if (Param.ParamsCount == 1) {
        XBytes Buf, Dest;
        Buf.resize(1048576);
        XIndexType ReadSize = fread(Buf.data(), 1, 1048576, fp);

        if (!ReadSize) {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::HeapPointer,
                            (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                    {EnvObject::ObjectKind::ClassObject,
                                     (EnvObject::ObjectValue) ConstructInternalErrorStructure(Interpreter,
                                                                                              L"FileError",
                                                                                              L"Cannot read from file.")
                                    })
                    });

            Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
            return;
        }
        while (ReadSize == 1048576) {
            Dest += Buf;
            ReadSize = fread(Buf.data(), 1, 1048576, fp);
        }
        Dest += Buf.substr(0, ReadSize);
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        EnvironmentStackItem::ItemKind::HeapPointer,
                        (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                {EnvObject::ObjectKind::BytesObject,
                                 (EnvObject::ObjectValue) CreateEnvBytesObjectFromXBytes(Dest)})
                });

        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
    } else {
        auto Count = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();

        auto BytesObject = CreateEnvBytesObject(Count.Value.IntVal);
        XIndexType Block = fread(&BytesObject->Dest, 1, Count.Value.IntVal, fp);
        if (Block) {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::HeapPointer,
                            (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                    {EnvObject::ObjectKind::BytesObject, (EnvObject::ObjectValue) BytesObject})
                    });
        } else {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::HeapPointer,
                            (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                    {EnvObject::ObjectKind::ClassObject,
                                     (EnvObject::ObjectValue) ConstructInternalErrorStructure(Interpreter,
                                                                                              L"FileError",
                                                                                              L"Cannot read from file.")
                                    })
                    });
        }

        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
    }
}

void File_write(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XScript::EnvClassObject *This = Interpreter->InterpreterEnvironment->Heap.HeapData[
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.Elements[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From
            ].Value.HeapPointerVal].Value.ClassObjectPointer;

    auto fp = (FILE *) (Interpreter->InterpreterEnvironment->Heap.HeapData[This->Members[Hash(
            L"__file__")]].Value.BytesObjectPointer);

    if (fp == nullptr) {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        EnvironmentStackItem::ItemKind::HeapPointer,
                        (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                {EnvObject::ObjectKind::ClassObject,
                                 (EnvObject::ObjectValue) ConstructInternalErrorStructure(Interpreter,
                                                                                          L"FileError",
                                                                                          L"Null file pointer.")
                                })
                });
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
        return;
    }

    auto &ToWrite =
            Interpreter->InterpreterEnvironment->Heap.HeapData[
                    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.HeapPointerVal];

    XInteger Res = 0;

    switch (ToWrite.Kind) {
        case XScript::EnvObject::ObjectKind::ClassObject: {
            if (ToWrite.Value.ClassObjectPointer->Members.count(Hash(L"__buffer__"))) {
                Res = fwrite(wstring2string(CovertToXString(
                                     Interpreter->InterpreterEnvironment->Heap.HeapData[ToWrite.Value.ClassObjectPointer->Members[Hash(
                                             L"__buffer__")]].Value.StringObjectPointer)).c_str(),
                             Interpreter->InterpreterEnvironment->Heap.HeapData[ToWrite.Value.ClassObjectPointer->Members[Hash(
                                     L"__buffer__")]].Value.StringObjectPointer->Length,
                             1,
                             fp);
            } else {
                Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                        {
                                EnvironmentStackItem::ItemKind::HeapPointer,
                                (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                        {EnvObject::ObjectKind::ClassObject,
                                         (EnvObject::ObjectValue) ConstructInternalErrorStructure(Interpreter,
                                                                                                  L"FileError",
                                                                                                  L"Invalid object to write")
                                        })
                        });
                Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
                return;
            }
            break;
        }
        case XScript::EnvObject::ObjectKind::StringObject: {
            Res = fwrite(
                    wstring2string(CovertToXString(ToWrite.Value.StringObjectPointer)).c_str(),
                    ToWrite.Value.StringObjectPointer->Length,
                    1,
                    fp);
            break;
        }
        case XScript::EnvObject::ObjectKind::BytesObject: {
            Res = fwrite(
                    &ToWrite.Value.BytesObjectPointer->Dest,
                    ToWrite.Value.BytesObjectPointer->Length,
                    1,
                    fp);
            break;
        }
        default: {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::HeapPointer,
                            (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                    {EnvObject::ObjectKind::ClassObject,
                                     (EnvObject::ObjectValue) ConstructInternalErrorStructure(Interpreter,
                                                                                              L"FileError",
                                                                                              L"Invalid object to write")
                                    })
                    });
            Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
            return;
        }
    }

    if (!Res) {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {
                        EnvironmentStackItem::ItemKind::HeapPointer,
                        (EnvironmentStackItem::ItemValue) Interpreter->InterpreterEnvironment->Heap.PushElement(
                                {EnvObject::ObjectKind::ClassObject,
                                 (EnvObject::ObjectValue) ConstructInternalErrorStructure(Interpreter,
                                                                                          L"FileError",
                                                                                          L"Cannot write to file.")
                                })
                });
    } else {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XInteger>(0)});
    }

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XIndexType) {0}});
}

void FS_exists(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    EnvStringObject *FilePath = GetStringObject(
            *Interpreter,
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());

    if (!FilePath) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"FSError", L"Invalid path."));
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
        return;
    }

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::Boolean,
                    (EnvironmentStackItem::ItemValue) std::filesystem::exists(wstring2string(CovertToXString(FilePath)))
            }
    );

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
}

void FS_isDirectory(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    EnvStringObject *FilePath = GetStringObject(
            *Interpreter,
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());

    if (!FilePath) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"FSError", L"Invalid path."));
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
        return;
    }

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::Boolean,
                    (EnvironmentStackItem::ItemValue) std::filesystem::is_directory(
                            wstring2string(CovertToXString(FilePath)))
            }
    );

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
}

void FS_listDirectory(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    EnvStringObject *FilePath = GetStringObject(
            *Interpreter,
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());

    if (!FilePath) {
        PushClassObjectStructure(Interpreter,
                                 ConstructInternalErrorStructure(Interpreter, L"FSError", L"Invalid path."));
        Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
        return;
    }
    XArray<XIndexType> StringArray;
    for (auto &I: std::filesystem::directory_iterator(wstring2string(CovertToXString(FilePath)))) {
        StringArray.push_back(
                Interpreter->InterpreterEnvironment->Heap.PushElement(
                        (EnvObject) {
                                XScript::EnvObject::ObjectKind::StringObject,
                                (EnvObject::ObjectValue) CreateEnvStringObjectFromXString(
                                        string2wstring(I.path().string()))
                        }
                ));
    }
    EnvArrayObject *Object = NewEnvArrayObject(0);
    Object->Elements = StringArray;
    XHeapIndexType Int = Interpreter->InterpreterEnvironment->Heap.PushElement(
            (EnvObject) {
                    XScript::EnvObject::ObjectKind::ArrayObject,
                    (EnvObject::ObjectValue) Object
            }
    );
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            (EnvironmentStackItem) {
                    XScript::EnvironmentStackItem::ItemKind::HeapPointer,
                    (EnvironmentStackItem::ItemValue) Int
            }
    );
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
}
