cmake_minimum_required(VERSION 3.12)
project(apush)
include_directories(include)
set(CMAKE_C_STANDARD 11)

add_executable(apush
    src/builtins.c
    src/exec.c
    src/heap.c
    src/interpreter.c
    src/main.c
    src/operations.c
    src/parser.c
    src/stack.c
)