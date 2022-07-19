//
// Created by chou on 22-7-18.
//

#include <iostream>
#include <dlfcn.h>

int main () {
    void * n = dlopen("/home/chou/Projects/XScript2Stdlib/cmake-build-debug/libio.xnc.so", RTLD_LAZY);
    if (n) {
        void * ptr = dlsym(n, "Initializer");
        while (1);
    } else {
        return 1;
    }
}