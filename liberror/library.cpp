#include <iostream>

#include "library.h"
#include "../Share/Utils.hpp"

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"isError")] = {2, Error_isError};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"Error")] = {L"Error", Methods};
    return {
            L"Jerry Chou",
            L"XScript 2 LibError",
            Classes};
}

void Error_isError(XScript::ParamToMethod Method) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Method.InterpreterPointer);

    auto StackE = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    if (StackE.Kind == XScript::EnvironmentStackItem::ItemKind::HeapPointer) {
        auto &HeapE = Interpreter->InterpreterEnvironment->Heap.HeapData[
                StackE.Value.HeapPointerVal];
        if (HeapE.Kind == XScript::EnvObject::ObjectKind::ClassObject) {
            if (HeapE.Value.ClassObjectPointer->Self == (ClassDescriptor) {Hash(L"Error"), Hash(L"Error")}) {
                Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                        {EnvironmentStackItem::ItemKind::Boolean, (EnvironmentStackItem::ItemValue) true});
            } else {
                Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                        {EnvironmentStackItem::ItemKind::Boolean, (EnvironmentStackItem::ItemValue) false});
            }
        } else {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {EnvironmentStackItem::ItemKind::Boolean, (EnvironmentStackItem::ItemValue) false});
        }
    } else {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                {EnvironmentStackItem::ItemKind::Boolean, (EnvironmentStackItem::ItemValue) false});
    }
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) 0});
}
