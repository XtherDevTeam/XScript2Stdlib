#include "library.h"
#include "../XScript2/Executor/BytecodeInterpreterPool.hpp"
#include "../Share/Utils.hpp"

void xscript2_thread_wrapper(XScript::BytecodeInterpreter *Interpreter, XScript::EnvironmentStackItem Func,
                             const XScript::XArray<XScript::EnvironmentStackItem> &Params) {
    using namespace XScript;
    auto InterpreterPool = static_cast<BytecodeInterpreterPool *>(Interpreter->Pool);

    Interpreter->IsBusy = true;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].IsBusy = true;

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack = {};
    // for the result
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.push_back(
            {EnvironmentStackFramesInformation::FrameKind::FunctionStackFrame,
             0,
             0,
             {}
            });
    // for the function
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.FramesInformation.push_back(
            {EnvironmentStackFramesInformation::FrameKind::FunctionStackFrame,
             0,
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
}

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"start")] = {2, start};
    Methods[XScript::Hash(L"id")] = {1, id};
    Methods[XScript::Hash(L"getResult")] = {2, getResult};
    Methods[XScript::Hash(L"sleep_ms")] = {2, sleep_ms};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"Thread")] = {L"Thread", Methods};
    return {
            L"Jerry Chou",
            L"XScript 2 LibThread",
            Classes};
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
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {
                            EnvironmentStackItem::ItemKind::Integer,
                            (EnvironmentStackItem::ItemValue) {static_cast<XInteger>(NewThreadId)}
                    });
            Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {static_cast<XHeapIndexType>(0)});
            break;
        }
        default:
            PushClassObjectStructure(
                    Interpreter,
                    ConstructInternalErrorStructure(Interpreter, L"ThreadError",
                                                    L"The first params isn't a callable object."));
            break;
    }
}

void id(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                    EnvironmentStackItem::ItemKind::Integer,
                    (EnvironmentStackItem::ItemValue) {static_cast<XInteger>(Interpreter->ThreadID)}
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

void sleep_ms(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    EnvironmentStackItem Duration = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();

    std::this_thread::sleep_for(std::chrono::milliseconds(Duration.Value.IntVal));
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {static_cast<XHeapIndexType>(0)});
}