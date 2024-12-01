cmake_minimum_required(VERSION 3.23)
project(lexical-parser-builder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
        lexical-parser-builder
        src/BuilderMain.cpp
        src/RegexScanner.cpp
        src/DFAGenerator.cpp
        src/TransitionTableContainer.cpp
)

add_executable(
        lexical-analyzer
        src/AnalyzerMain.cpp
        src/TransitionTableContainer.cpp
        src/LexicalAnalyzer.cpp
)

# Related to GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        lexical-parser-builder-testing
        testing/DFAGeneratorTests.cpp
        src/DFAGenerator.cpp
        src/TransitionTableContainer.cpp
        testing/TransitionTableContainerTests.cpp
)
target_link_libraries(
        lexical-parser-builder-testing
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lexical-parser-builder-testing)