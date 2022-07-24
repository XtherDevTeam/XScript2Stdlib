#include <iostream>

#include "library.h"


extern "C" XScript::NativeClassInformation Initialize() {
    XScript::XMap<XScript::XIndexType, XScript::NativeMethodInformation> Methods;
    return {
            L"Jerry Chou",
            L"XScript 2 LibArray",
            Methods};
}