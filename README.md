# OP_Y2S2M2
Compilation
To compile the program, navigate to the directory where the source code file is saved and run the following command in the terminal:
make



#cmp tool:

Usage and System Environment
This program is a command-line tool that compares two files and checks whether they are equal or not. The program is written in C language and can be compiled and run on a Linux system.

Usage
Once the program is compiled, you can run it using the following command:
./cmp <file1> <file2> [-v] [-i]

The first two arguments are the paths to the files that you want to compare. The optional flags are:

-v: Enables verbose output. If the files are equal, the program will output "equal". If the files are different, the program will output "distinct".
-i: Enables case-insensitive comparison. This means that if this flag is enabled, the program will treat uppercase and lowercase characters as the same.
Example
Suppose you want to compare two files called file1.txt and file2.txt that are saved in the same directory as the cmp program. To do this, you would run the following command:

./cmp file1.txt file2.txt

Whether they are equal or not the program will not emit anything but will return 0 or 1 respectively.

If you want to enable verbose output, you would run the following command:
./cmp file1.txt file2.txt -v

If the files are equal, the program will output "equal". If the files are different, the program will output "distinct".

If you want to enable case-insensitive comparison, you would run the following command:
./cmp file1.txt file2.txt -i

If you want to enable both verbose output and case-insensitive comparison, you would run the following command:
./cmp file1.txt file2.txt -v -i







# Copy Tool 

Usage
The Copy Tool is a command line tool that copies a file from a source file to a destination file. The tool allows the user to specify two types of flags:

-v flag: This flag enables verbose mode and outputs "success" if the file is copied, "target file exists" if the destination file already exists, or "general failure" if some other problem occurs.
-f flag: This flag allows the tool to overwrite the destination file if it already exists.
To use the Copy Tool, follow these steps:

Open a terminal and navigate to the directory where the tool is located.

Type the following command:
./copy <src_file> <dest_file> [-v] [-f]
where <src_file> is the name of the file you want to copy from, and <dest_file> is the name of the file you want to copy to. The -v and -f flags are optional.

For example, to copy a file named "file.txt" to a file named "file_copy.txt" with verbose mode enabled, the command would be:
./copy file.txt file_copy.txt -v
Press Enter to execute the command.

If the operation was successful, the tool will output "success" (if verbose mode is enabled) or exit silently. If the destination file already exists and the -f flag is not enabled, the tool will output "target file exists" (if verbose mode is enabled). If an error occurred, the tool will output "general failure" (if verbose mode is enabled).




# codecA
The codecA library is a coding library that contains 2 functions:
Encoding - converts any lowercase letter to uppercase letter and vice versa
Decoding - does the reverse of the encoding operation
This code consists of two functions "encode" and "decode" that can be used to convert text from lowercase to uppercase and vice versa.

# codecB
The codecB library is a coding library that contains 2 functions:
Encoding - 3 is added to each letter to its ascii value
Decoding - does the reverse of the encoding operation

This code consists of two functions "encode" and "decode" that can be used to convert text.



# decode tool
Usage and System Environment:
This program is a command-line tool that decodes a message using a codec library. The program is written in C language and can be compiled and run on a Linux system. The tool uses dynamic linking to load the codec library at runtime.

Usage:
Once the program is compiled, you can run it using the following command:
./decoder <codec> <message>

The first argument is the name of the codec library you want to use. The second argument is the message you want to decode. The supported codecs are codecA and codecB.

Example:
Suppose you want to decode a message "HELLO WORLD" using codecB. To do this, you would run the following command:

./decoder codecB "HELLO WORLD"

The program will load the codecB library using dynamic linking and call its decode function to decode the message. The decoded message will be printed to the console.

Note: If you provide an invalid codec name or an incorrect number of arguments, the program will print a usage message and exit.




# decode tool
This program is a command-line tool written in C language that encodes a message using either "codecA" or "codecB", which are two shared libraries. The program can be compiled and run on a Linux system.

Usage

Once the program is compiled and the shared libraries are available, you can run it using the following command:

./encoder <codec> <message>

The first argument specifies which codec to use, either "codecA" or "codecB". The second argument is the message to be encoded.

Example

Suppose you want to encode a message "Hello, world!" using "codecA". To do this, you would run the following command:

./encoder codecA "Hello, world!"

The program will then encode the message using "codecA" and output the encoded message to the console.

If you want to encode the message using "codecB", you would run the following command:

./encoder codecB "Hello, world!"

If the specified codec is not valid or there is an error loading the shared library, the program will output an error message and return a non-zero exit code.







System Environment
The program was developed and tested on a system running Ubuntu 20.04 LTS.
