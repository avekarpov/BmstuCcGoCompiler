cmake_minimum_required(VERSION 3.20.0)
project(go_compiler LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(codegen)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})

set(LLVM_DIR "/usr/local/opt/llvm/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
        lib/antlr4_runtime/include
        ${LLVM_INCLUDE_DIRS}
        include/antlr4/codegen
        include
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/lib/antlr4_runtime/bin/libantlr4-runtime.a
        LLVM
)

add_subdirectory(test)
