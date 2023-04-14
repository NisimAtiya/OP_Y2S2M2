#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dlfcn.h>



int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <codec> <message>\n", argv[0]);
        return 1;
    }
    typedef void (*CodecFunc)(char *);
    CodecFunc func;
    void *handle;


    if (strcmp(argv[2], "codecA") == 0) {
        handle = dlopen("./libcodecA.so", RTLD_LAZY);
    } else if (strcmp(argv[2], "codecB") == 0) {
        handle = dlopen("./libcodecB.so", RTLD_LAZY);
    } else{
        printf("Usage: %s <codec> <message>\n", argv[0]);
        return 1;
    }

    if (!handle) {
        printf("Error: %s\n", dlerror());
        return 1;
    }
    func = (CodecFunc) dlsym(handle, "encode");
    if (!func) {
        dlclose(handle);
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }
    (*func)(argv[3]);
    return 1;
}

