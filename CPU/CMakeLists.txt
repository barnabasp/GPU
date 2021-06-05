cmake_minimum_required(VERSION 3.0.0)
project(Conway VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Conway main.cpp funcs.cpp)
add_executable(ConwayCPU main_cpu_par.cpp funcs.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
                                               $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)