cmake_minimum_required(VERSION 3.4)
project(apush)
include_directories(include)
set(CMAKE_C_STANDARD 11)
set(CMAKE_INSTALL_PREFIX /usr/local/bin)
set(CMAKE_SOURCE_DIR cmake)

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

install(TARGETS apush DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(CODE "execute_process(COMMAND ${CMAKE_SOURCE_DIR}/PostInstall.sh ${CMAKE_INSTALL_PREFIX})")