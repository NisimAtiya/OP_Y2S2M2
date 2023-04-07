#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

int cmp(const char* file1, const char* file2, int verbose, int ignore_case) {
    FILE *f1, *f2;
    char c1, c2;

    f1 = fopen(file1, "r");
    if (f1 == NULL) {
        printf("Error: failed to open %s\n", file1);
        return 2;
    }

    f2 = fopen(file2, "r");
    if (f2 == NULL) {
        printf("Error: failed to open %s\n", file2);
        fclose(f1);
        return 2;
    }
    // Calculate the size of file1
    struct stat st1;
    int fd1 = fileno(file1);    // get file descriptor
    fstat(fd1, &st1);
    off_t size_file1 = st1.st_size;
    // Calculate the size of file2
    struct stat st2;
    int fd2 = fileno(file1);    // get file descriptor
    fstat(fd2, &st2);
    off_t size_file2= st2.st_size;
    //Checks if the sizes are the same
    if (size_file1!=size_file2){
        fclose(f1);
        fclose(f2);
        if (verbose) {
            printf("distinct\n");
        }
        return 1;
    }


    while ((c1 = fgetc(f1)) != EOF || (c2 = fgetc(f2)) != EOF) {
        if (ignore_case) {
            if( (c2>'a' && c1<'z' || c1>'A' && c1<'Z') && (c2>'a'&&c1<'z'||c1>'A'&&c1<'Z') ){
                c1 = tolower(c1);
                c2 = tolower(c2);
            }


        }
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            if (verbose) {
                printf("nisim\n");
                printf("distinct\n");
            }
            return 1;
        }
    }

    if (c1 != EOF || c2 != EOF) {
        fclose(f1);
        fclose(f2);
        if (verbose) {
            printf("atiya\n");
            printf("distinct\n");
        }
        return 1;
    }

    fclose(f1);
    fclose(f2);

    if (verbose) {
        printf("equal\n");
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int ignore_case = 0;

    if (argc < 3 || argc > 5) {
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return 2;
    }

    if (argc == 4) {
        if (strcmp(argv[3], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[3], "-i") == 0) {
            ignore_case = 1;
        } else {
            printf("Unknown flag: %s\n", argv[3]);
            return 2;
        }
    }

    if (argc == 5) {
        if (strcmp(argv[3], "-v") == 0 && strcmp(argv[4], "-i") == 0) {
            verbose = 1;
            ignore_case = 1;
        } else if (strcmp(argv[3], "-i") == 0 && strcmp(argv[4], "-v") == 0) {
            verbose = 1;
            ignore_case = 1;
        } else {
            printf("Unknown combination of flags: %s %s\n", argv[3], argv[4]);
            return 2;
        }
    }

    return cmp(argv[1], argv[2], verbose, ignore_case);
}
