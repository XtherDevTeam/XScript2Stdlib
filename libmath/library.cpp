#include <iostream>
#include <climits>

#include "library.h"
#include "../Share/Utils.hpp"
#include <numbers>
#include <cmath>

std::random_device RNG;

extern "C" XScript::NativeLibraryInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    Methods[XScript::Hash(L"e")] = {0, Math_e};
    Methods[XScript::Hash(L"pi")] = {0, Math_pi};
    Methods[XScript::Hash(L"log2e")] = {0, Math_log2e};
    Methods[XScript::Hash(L"log10e")] = {0, Math_log10e};
    Methods[XScript::Hash(L"sqrt2")] = {0, Math_sqrt2};
    Methods[XScript::Hash(L"sqrt3")] = {0, Math_sqrt3};
    Methods[XScript::Hash(L"sqrt")] = {0, Math_sqrt};
    Methods[XScript::Hash(L"floor")] = {0, Math_floor};
    Methods[XScript::Hash(L"ceil")] = {0, Math_ceil};
    Methods[XScript::Hash(L"trunc")] = {0, Math_trunc};
    Methods[XScript::Hash(L"gcd")] = {0, Math_gcd};
    Methods[XScript::Hash(L"abs")] = {0, Math_abs};
    Methods[XScript::Hash(L"sin")] = {0, Math_sin};
    Methods[XScript::Hash(L"cos")] = {0, Math_cos};
    Methods[XScript::Hash(L"tan")] = {0, Math_tan};
    Methods[XScript::Hash(L"asin")] = {0, Math_asin};
    Methods[XScript::Hash(L"acos")] = {0, Math_acos};
    Methods[XScript::Hash(L"atan")] = {0, Math_atan};

    XScript::XMap<XScript::XIndexType, XScript::NativeClassInformation> Classes;
    Classes[XScript::Hash(L"Math")] = {L"Math", Methods};
    return {
            L"Jerry Chou",
            L"XScript 2 LibMath",
            Classes};
}

void Math_e(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XDecimal Deci = std::numbers::e_v<XDecimal>;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) Deci});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_pi(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XDecimal Deci = std::numbers::pi_v<XDecimal>;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) Deci});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_log2e(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XDecimal Deci = std::numbers::log2e_v<XDecimal>;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) Deci});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_log10e(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XDecimal Deci = std::numbers::log10e_v<XDecimal>;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) Deci});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_sqrt2(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XDecimal Deci = std::numbers::sqrt2_v<XDecimal>;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) Deci});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_sqrt3(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);
    XDecimal Deci = std::numbers::sqrt3_v<XDecimal>;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) Deci});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_sqrt(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) sqrt(Item)});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_floor(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) floor(Item)});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_ceil(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) ceil(Item)});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_trunc(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto Item = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Decimal, (EnvironmentStackItem::ItemValue) trunc(Item)});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_gcd(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    auto y = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    if (x < y) {
        x ^= y;
        y ^= x;
        x ^= y;
    }
    while (y) {
        XInteger r = x % y;
        x = y;
        y = r;
    }

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) x});
    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_abs(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack();
    switch (x.Kind) {
        case XScript::EnvironmentStackItem::ItemKind::Integer:
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue)
                            (x.Value.IntVal ? x.Value.IntVal : -x.Value.IntVal)});
            break;
        case XScript::EnvironmentStackItem::ItemKind::Decimal:
            Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
                    {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue)
                            (x.Value.DeciVal ? x.Value.DeciVal : -x.Value.DeciVal)});
            break;
        default:
            break;
    }

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_sin(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XDecimal>(sin(x))});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_cos(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XDecimal>(cos(x))});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_tan(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XDecimal>(tan(x))});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_asin(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XDecimal>(asin(x))});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_acos(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XDecimal>(acos(x))});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_atan(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    auto x = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.DeciVal;
    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {EnvironmentStackItem::ItemKind::Integer, (EnvironmentStackItem::ItemValue) static_cast<XDecimal>(atan(x))});

    Interpreter->InstructionFuncReturn((BytecodeStructure::InstructionParam) {(XInteger) {}});
}

void Math_random(XScript::ParamToMethod Param) {
    using namespace XScript;
    auto Interpreter = static_cast<BytecodeInterpreter *>(Param.InterpreterPointer);

    XInteger Start = LONG_MIN, End = LONG_MAX;
    if (Param.ParamsCount == 3) {
        End = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
        Start = Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PopValueFromStack().Value.IntVal;
    }
    std::uniform_int_distribution<XInteger> Dist(Start, End);

    Interpreter->InterpreterEnvironment->Threads[Interpreter->ThreadID].Stack.PushValueToStack(
            {
                EnvironmentStackItem::ItemKind::Integer,
                (EnvironmentStackItem::ItemValue) static_cast<XInteger>(Dist(RNG))});
}

