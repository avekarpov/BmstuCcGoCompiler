cmake_minimum_required(VERSION 3.20.0)
project(go_compiler_test LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(Catch2)

file(GLOB_RECURSE TESTS_SRC "tests/*.cpp")
file(GLOB_RECURSE ANTRL4_SRC "../src/antlr4/*.cpp")

add_executable(
    go_compiler_test 
    ${TESTS_SRC}
    ${ANTRL4_SRC}
    ../src/llvm_visitor.cpp
)

set(LLVM_DIR "/usr/local/opt/llvm/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

target_include_directories(
    go_compiler_test
    PRIVATE 
        ../include
        ../lib/antlr4_runtime/include
        ${LLVM_INCLUDE_DIRS}
        ../include/antlr4/codegen
)

target_link_libraries(
    go_compiler_test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../lib/antlr4_runtime/bin/libantlr4-runtime.a
        LLVM
    PUBLIC 
        Catch2::Catch2
)
