cmake_minimum_required(VERSION 3.23)
project(LexicalParser)

set(CMAKE_CXX_STANDARD 17)

# Set the path to Graphviz headers and libraries
set(GRAPHVIZ_INCLUDE_DIR "/opt/homebrew/Cellar/graphviz/12.2.0/include/graphviz")
set(GRAPHVIZ_LIB_DIR "/opt/homebrew/Cellar/graphviz/12.2.0/lib")

# Include directories and libraries for Graphviz
include_directories(${GRAPHVIZ_INCLUDE_DIR})
link_directories(${GRAPHVIZ_LIB_DIR})
link_directories(/opt/homebrew/Cellar/graphviz/12.2.0/lib)

# Add your executable and include the source files
add_executable(LexicalParser main.cpp RegexScanner.cpp NFA.cpp)

# Link the Graphviz libraries (adjust the library names as necessary)
target_link_libraries(LexicalParser
        /opt/homebrew/Cellar/graphviz/12.2.0/lib/libgvc.dylib
        /opt/homebrew/Cellar/graphviz/12.2.0/lib/libcdt.dylib
        /opt/homebrew/Cellar/graphviz/12.2.0/lib/libxdot.dylib)

