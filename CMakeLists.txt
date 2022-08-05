cmake_minimum_required(VERSION 3.0)
project(XScript2Stdlib)

set(CMAKE_CXX_STANDARD 17)

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

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-g")
endif()

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_SHARED_LINKER_FLAGS "-static")
endif ()

add_library(io SHARED libio/library.cpp ${SRC_FILES})
add_library(string SHARED libstring/library.cpp ${SRC_FILES})
add_library(array SHARED libarray/library.cpp ${SRC_FILES})
add_library(thread SHARED libthread/library.cpp ${SRC_FILES})
add_library(fs SHARED libfs/library.cpp ${SRC_FILES})
add_library(sys SHARED libsys/library.cpp ${SRC_FILES})