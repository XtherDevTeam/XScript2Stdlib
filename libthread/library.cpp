#include "library.h"
#include "../XScript2/Executor/BytecodeInterpreterPool.hpp"

void xscript2_thread_wrapper(XScript::BytecodeInterpreter *Interpreter, XScript::EnvironmentStackItem Func,
                             const XScript::XArray<XScript::EnvironmentStackItem> &Params) {
    using namespace XScript;
    auto InterpreterPool = static_cast<BytecodeInterpreterPool *>(Interpreter->Pool);

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.push_back(
            {EnvironmentStackFramesInformation::FrameKind::FunctionStackFrame,
             Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().From +
             Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.back().Length,
             0,
             {}
            });

    for (XIndexType Idx = Params.size() - 1; Idx + 1; Idx--) {
        Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(Params[Idx]);
    }

    switch (Func.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::FunctionPointer: {
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].PC = (ProgramCounterInformation) {
                    Func.Value.FuncPointerVal->BytecodeArray,
                    Func.Value.FuncPointerVal->PackageID};
            Interpreter->MainLoop();
            break;
        }
        case XScript::EnvironmentStackItem::ItemKind::NativeMethodPointer: {
            /* 被调用者需要手动调整栈帧 */
            Func.Value.NativeMethodPointerVal->Method({(void *) Interpreter});
            break;
        }
        default:
            // never run into here.
            break;
    }
    Interpreter->IsBusy = true;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].IsBusy = true;
}

XScript::EnvClassObject *
ConstructInternalErrorStructure(XScript::BytecodeInterpreter *Interpreter, const XScript::XString &ErrorName,
                                const XScript::XString &ErrorDescription) {
    using namespace XScript;
    EnvClassObject *Object = NewEnvClassObject();
    Object->Self = {Hash(L"Exceptions"), Hash(L"InternalError")};
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


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"start")] = {2, start};
    Methods[XScript::Hash(L"id")] = {1, id};
    Methods[XScript::Hash(L"getResult")] = {2, getResult};

    return {
            L"Jerry Chou",
            L"XScript 2 LibThread",
            Methods};
}

void start(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Param.ParamsCount -= 2;
    XArray<EnvironmentStackItem> Params;
    while (Param.ParamsCount--) {
        Params.push_back(Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack());
    }
    EnvironmentStackItem Func = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    auto InterpreterPool = static_cast<BytecodeInterpreterPool *>(Interpreter->Pool);
    switch (Func.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::FunctionPointer:
        case XScript::EnvironmentStackItem::ItemKind::NativeMethodPointer: {
            XIndexType NewThreadId = InterpreterPool->Allocate();
            Interpreter->InterpreterEnvironment->Threads[NewThreadId].Thread =
                    (std::thread) {xscript2_thread_wrapper, &(*InterpreterPool)[NewThreadId], Func, Params};
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack({
                                                                                                               EnvironmentStackItem::ItemKind::Integer,
                                                                                                               (EnvironmentStackItem::ItemValue) {
                                                                                                                       static_cast<XInteger>(NewThreadId)}
                                                                                                       });
            Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {static_cast<XHeapIndexType>(0)});
        }
        default:
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {EnvironmentStackItem::ItemKind::HeapPointer,
                     (EnvironmentStackItem::ItemValue) {
                             Interpreter->InterpreterEnvironment->Heap.PushElement(
                                     {EnvObject::ObjectKind::ClassObject,
                                      (EnvObject::ObjectValue) {ConstructInternalErrorStructure(Interpreter,
                                                                                                L"ThreadError",
                                                                                                L"The first params isn't a callable object.")}})}});
            break;
    }
}

void id(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack({
                                                                                                       EnvironmentStackItem::ItemKind::Integer,
                                                                                                       (EnvironmentStackItem::ItemValue) {
                                                                                                               static_cast<XInteger>(Interpreter->ThreadID)}
                                                                                               });
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {static_cast<XHeapIndexType>(0)});
}

void getResult(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    auto InterpreterPool = static_cast<BytecodeInterpreterPool *>(Interpreter->Pool);

    EnvironmentStackItem Idx = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();

    Interpreter->InterpreterEnvironment->Threads.WaitForThread(Idx.Value.HeapPointerVal);
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            Interpreter->InterpreterEnvironment->Threads[Idx.Value.HeapPointerVal].Stack.PopValueFromStack());

    (*InterpreterPool)[Idx.Value.HeapPointerVal].IsBusy = false;
    Interpreter->InterpreterEnvironment->Threads[Idx.Value.HeapPointerVal].IsBusy = false;

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {static_cast<XHeapIndexType>(0)});
}

