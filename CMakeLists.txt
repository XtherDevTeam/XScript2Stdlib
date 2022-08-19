cmake_minimum_required(VERSION 3.0)
project(XScript2Stdlib)

set(WHERE_IS_REPO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/XScript2)
include("${CMAKE_CURRENT_SOURCE_DIR}/XScript2/Share/Git.cmake")

set(GIT_COMMIT_HASH "")
get_git_hash(GIT_COMMIT_HASH)

set(CMAKE_CXX_STANDARD 20)

file(GLOB XS2_SRC_FILES
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

file(GLOB SHARE_SRC_FILES
        "Share/*.cpp")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-g")
endif()

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_SHARED_LINKER_FLAGS "-static")
endif ()

add_library(io SHARED libio/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(string SHARED libstring/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(array SHARED libarray/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(thread SHARED libthread/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(fs SHARED libfs/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(sys SHARED libsys/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(math SHARED libmath/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(error SHARED liberror/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(bytes SHARED libbytes/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})
add_library(hash SHARED libhash/library.cpp ${XS2_SRC_FILES} ${SHARE_SRC_FILES})

add_definitions(-DXSCRIPT2_COMMIT_VERSION=L\"${GIT_COMMIT_HASH}\")