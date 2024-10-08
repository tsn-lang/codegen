cmake_minimum_required(VERSION 3.0.0)
project(codegen_test VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${TSN_CODEGEN_INCLUDE_DIR}
    ${TSN_BIND_INCLUDE_DIR}
    ${TSN_UTILS_INCLUDE_DIR}
    ${FFI_INCLUDE_DIR}
)

if (NOT TARGET Catch2::Catch2)
    add_subdirectory(Catch2)
endif()

file(GLOB all_sources "./*.cpp" "../deps/bind/test/*.cpp")
list(FILTER all_sources EXCLUDE REGEX "\\.\\./deps/bind/test/main.cpp$")
add_executable(codegen_test ${all_sources})
target_link_libraries(codegen_test codegen Catch2::Catch2)