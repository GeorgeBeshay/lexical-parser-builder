cmake_minimum_required(VERSION 3.23)
project(LexicalParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(LexicalParser main.cpp RegexScanner.cpp)