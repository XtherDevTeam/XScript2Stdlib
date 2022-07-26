#ifndef XSCRIPT2STDLIB_LIBRARY_H
#define XSCRIPT2STDLIB_LIBRARY_H

#include "../XScript2/NativeLibrary/NativeLibrary.hpp"
#include "../XScript2/Executor/Environment.hpp"
#include "../XScript2/Executor/BytecodeInterpreter.hpp"

XScript::EnvClassObject *CloneFileObject(XScript::BytecodeInterpreter *Interpreter);

void File_open(XScript::ParamToMethod Param);

void File_close(XScript::ParamToMethod Param);

void File_read(XScript::ParamToMethod Param);

void File_write(XScript::ParamToMethod Param);

void File_getStdin(XScript::ParamToMethod Param);

void File_getStdout(XScript::ParamToMethod Param);

void FS_exists(XScript::ParamToMethod Param);

void FS_isDirectory(XScript::ParamToMethod Param);

void FS_listDirectory(XScript::ParamToMethod Param);

void FS_removeDirectories(XScript::ParamToMethod Param);

void FS_createDirectories(XScript::ParamToMethod Param);

void FS_removeDirectory(XScript::ParamToMethod Param);

void FS_createDirectory(XScript::ParamToMethod Param);

extern "C" XScript::NativeLibraryInformation Initialize();

#endif //XSCRIPT2STDLIB_LIBRARY_H
