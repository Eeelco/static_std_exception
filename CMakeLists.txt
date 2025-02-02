cmake_minimum_required(VERSION 3.10)

project(static_std_exception)
set(CMAKE_CXX_STANDARD 17)

project(static_std_exception)

add_subdirectory(static_exception)

add_library(static_std_exception SHARED lib/throw_functions.cc)
target_link_libraries(static_std_exception static_exception)

target_compile_options(static_std_exception PRIVATE -O2 -Wall -Werror -Wpedantic -DFORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fpic)

add_subdirectory(test)