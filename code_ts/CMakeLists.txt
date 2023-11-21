cmake_minimum_required(VERSION 3.25)
project(code_ts C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c library.c)

add_library(code_ts ${SOURCE_FILES})
