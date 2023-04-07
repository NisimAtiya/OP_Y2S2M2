#include <stdio.h>
#include <string.h>
#include <ctype.h>
// Function to calculate file size
long int get_file_size(const char* filename) {
    FILE* fp = fopen(filename, "rb"); // Open file in binary mode
    if (fp == NULL) {
        return -1; // Error opening file
    }
    fseek(fp, 0, SEEK_END); // Seek to end of file
    long int size = ftell(fp); // Get current position (which is the file size)
    fclose(fp); // Close file
    return size;
}

int cmp(const char* file1, const char* file2, int v, int ignore_case) {
    //Checks if the sizes are the same
    if (get_file_size(file1)!=get_file_size(file2)){
        if (v) {
            printf("distinct\n");
        }
        return 1;
    }
    // If the files are empty then they are equal
    if(get_file_size(file1)==0){
        if (v) {
            printf("equal\n");
        }
        return 0;
    }
    // opening the files
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


    for (long int i = 0; i <get_file_size(file1) ; ++i) {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        // changes to lower case if ignore_case is on
        if (ignore_case) {
            c1 = tolower(c1);
            c2 = tolower(c2);

        }
        // Checks if the chars are different
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            // Prints distinct if v is on and returns 1
            if (v) {
                printf("distinct\n");
            }
            return 1;
        }
    }
    // Prints equal if v is on and returns 0
    if (v) {
        printf("equal\n");
    }

    return 0;

}

int main(int argc, char *argv[]) {
    // bits for user input for -v and -i
    int v = 0;
    int ignore_case = 0;
    // checking if input is not correct
    if (argc < 3 || argc > 5) {
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return 2;
    }

    if (argc == 4) {
        if (strcmp(argv[3], "-v") == 0) {
            v = 1;
        } else if (strcmp(argv[3], "-i") == 0) {
            ignore_case = 1;
        } else {
            printf("Unknown flag: %s\n", argv[3]);
            return 2;
        }
    }

    if (argc == 5) {
        if (strcmp(argv[3], "-v") == 0 && strcmp(argv[4], "-i") == 0) {
            v = 1;
            ignore_case = 1;
        } else if (strcmp(argv[3], "-i") == 0 && strcmp(argv[4], "-v") == 0) {
            v = 1;
            ignore_case = 1;
        } else {
            printf("Unknown combination of flags: %s %s\n", argv[3], argv[4]);
            return 2;
        }
    }

    return cmp(argv[1], argv[2], v, ignore_case);
}