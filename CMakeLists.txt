cmake_minimum_required(VERSION 3.23)
project(lexical-parser-builder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
        lexical-parser-builder
        src/BuilderMain.cpp
        src/lexical-analyzer/utilities/RegexScanner.cpp
        src/lexical-analyzer/dfa/DFAGenerator.cpp
        src/lexical-analyzer/nfa/NFA.cpp
        src/lexical-analyzer/utilities/TransitionTableContainer.cpp
        src/lexical-analyzer/utilities/lexicalTypes.h
        src/lexical-analyzer/utilities/LexicalUtility.cpp
        src/parser/grammar/Grammar.cpp
        src/parser/grammar/GrammarRule.cpp
        src/parser/utilities/GrammarUtility.cpp
        src/parser/utilities/parsingTypes.h
        src/parser/utilities/ParsingToken.cpp
        src/parser/utilities/ParserUtility.cpp
        src/parser/parsing-table/ParsingTable.cpp
)

add_executable(
        lexical-parser-runner
        src/RunnerMain.cpp
        src/lexical-analyzer/utilities/TransitionTableContainer.cpp
        src/lexical-analyzer/LexicalAnalyzer.cpp
        src/parser/utilities/parsingTypes.h
        src/parser/utilities/ParsingToken.cpp
        src/parser/utilities/ParserUtility.cpp
        src/parser/utilities/GrammarUtility.cpp
        src/parser/parsing-table/ParsingTable.cpp
        src/parser/ParsingAnalyzer.cpp
)

# Related to GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        lexical-parser-builder-testing
        testing/DFAGeneratorTests.cpp
        src/lexical-analyzer/dfa/DFAGenerator.cpp
        src/lexical-analyzer/nfa/NFA.cpp
        src/lexical-analyzer/utilities/TransitionTableContainer.cpp
        testing/TransitionTableContainerTests.cpp
        src/lexical-analyzer/utilities/lexicalTypes.h
        src/lexical-analyzer/utilities/LexicalUtility.cpp
        testing/LexicalAnalyzerTests.cpp
        src/parser/utilities/ParsingToken.cpp
        testing/ParsingTokenTests.cpp
        src/parser/grammar/Grammar.cpp
        src/parser/grammar/GrammarRule.cpp
        src/parser/utilities/ParserUtility.cpp
        src/parser/utilities/GrammarUtility.cpp
        testing/ParsingFirstFollowSetTests.cpp
        src/parser/parsing-table/ParsingTable.cpp
        testing/ParsingTableTests.cpp
        src/parser/ParsingAnalyzer.cpp
        testing/ParsingAnalyzerTests.cpp
)
target_link_libraries(
        lexical-parser-builder-testing
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lexical-parser-builder-testing)