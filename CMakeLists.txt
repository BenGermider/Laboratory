cmake_minimum_required(VERSION 3.27)
project(Laboraty C)

set(CMAKE_C_STANDARD 90)

add_executable(Laboraty
        myset.c
        set.c
        set.h
        myset.h
)

target_compile_options(Laboraty PRIVATE -ansi -Wall -pedantic)