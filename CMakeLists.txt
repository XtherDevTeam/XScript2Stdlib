cmake_minimum_required(VERSION 3.23)
project(XScript2Stdlib)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC_FILES
        "XScript2/Frontend/*.cpp"
        "XScript2/Frontend/Parsers/*.cpp"
        "XScript2/Backend/*.cpp"
        "XScript2/Backend/Compiler/*.cpp"
        "XScript2/Executor/*.cpp"
        "XScript2/Share/*.cpp"
        "XScript2/Share/Exceptions/*.cpp"
        "XScript2/Core/*.cpp"
        "XScript2/Core/Serializatior/*.cpp"
        "XScript2/Core/Reader/*.cpp"
        "XScript2/NativeLibrary/*.cpp"
)

set(CMAKE_DEBUG_POSTFIX ".xnc")
set(CMAKE_RELEASE_POSTFIX ".xnc")

add_definitions("-g")

add_library(io SHARED libio/library.cpp ${SRC_FILES})
add_library(string SHARED libstring/library.cpp ${SRC_FILES})
add_library(array SHARED libarray/library.cpp ${SRC_FILES})
add_executable(main main.cpp)