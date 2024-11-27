cmake_minimum_required(VERSION 3.23)
project(lexical-parser-builder)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        lexical-parser-builder
        src/main.cpp
        src/RegexScanner.cpp
)