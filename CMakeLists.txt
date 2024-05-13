cmake_minimum_required(VERSION 3.20.0)
project(go_compiler LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(codegen)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
        lib/antlr4_runtime/include
        include/antlr4/codegen
        include
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/lib/antlr4_runtime/bin/libantlr4-runtime.a
)
