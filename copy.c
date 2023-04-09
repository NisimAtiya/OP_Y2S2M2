#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
int copy_file(const char* src_file, const char* dest_file, int v, int f) {
    // open the source file
    FILE* src = fopen(src_file, "rb");

    // print a message if I could not open the file and if v is on
    if (!src) {
        if (v) {
            printf("general failure: cannot open source file '%s'.\n", src_file);
        }
        return 1;
    }

    // open the target file according to the flag f
    FILE* dest;
    if (f) {
        dest = fopen(dest_file, "wb");
    } else {
        dest = fopen(dest_file, "ab");
    }

    // print a message if I could not open the file and if v is on
    if (!dest) {
        if (v) {
            printf("general failure: cannot open destination file '%s'.\n", dest_file);
        }
        fclose(src);
        return 1;
    }

    int result = 0;     // This variable will be used to indicate whether an error occurred during the code.

    // moves the pointer of dest to the end of the file and checks if I succeeded.
    if (fseek(dest, 0, SEEK_END) != 0) {
        result = 1;
    } else {
        //c alculates the file size
        long dest_size = ftell(dest);
        //returns dest's pointer to the beginning of the file.
        fseek(dest, 0, SEEK_SET);
        // checks if something exists in the dest file.
        if (dest_size > 0) {
            // if so, and the v flag is on, it prints a message accordingly.
            if (v) {
                printf("target file exists\n");
            }
            // if the f flag is not lit, the result is updated to 1.
            if (!f) {
                result = 1;
            }
        }
    }
    // if no error happened until now, copy the src file to the dest file.
    if (!result) {
        int c;
        while ((c = fgetc(src)) != EOF) {
            fputc(c, dest);
        }

        if (ferror(src) || ferror(dest)) {
            result = 1;
        }
    }
    // closes the src and dest file
    fclose(src);
    fclose(dest);
    // if v is on and result is equal to 0, print: "success", if not print: "general failure".
    if (v) {
        if (result == 0) {
            printf("success\n");
        } else {
            printf("general failure\n");
        }
    }

    return result;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("To use the copy tool:\n"
               "You must enter two file names where the first is the name of the file from which you want to copy and the second file is the name of the file to which you want to copy.\n"
               "in the following form: ./copy <src_file> <dest_file>\n"
               "It is possible to insert two types of flags:\n"
               "1. [-v] which will output \"success\" if the file is copied, or \"target file exists\" if this is the case, or \"general failure\" in some other problem/\n"
               "2. [-f] which allows to overwrite the target file.\n");
        return 1;
    }
    if (access(argv[1], F_OK) == -1) {
        printf("To use the copy tool:\n"
               "You must enter two file names where the first is the name of the file from which you want to copy and the second file is the name of the file to which you want to copy.\n"
               "in the following form: ./copy <src_file> <dest_file>\n"
               "It is possible to insert two types of flags:\n"
               "1. [-v] which will output \"success\" if the file is copied, or \"target file exists\" if this is the case, or \"general failure\" in some other problem/\n"
               "2. [-f] which allows to overwrite the target file.\n");
        return 1;
    }

    const char* src_file = argv[1];
    const char* dest_file = argv[2];
    int v = 0;
    int f = 0;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            v = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            f = 1;
        }
    }

    return copy_file(src_file, dest_file, v, f);
}