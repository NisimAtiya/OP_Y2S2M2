cmake_minimum_required(VERSION 3.25)
project(OP_Y2S2M2 C)

set(CMAKE_C_STANDARD 11)

add_executable(OP_Y2S2M2
        cmp.c copy.c codecA.c codecB.c decode.c encode.c myshell.c)
