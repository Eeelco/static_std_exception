find_package(GTest REQUIRED)
enable_testing()

include_directories(${GTEST_INCLUDE_DIRS})
set(TESTNAME static_std_exception_test)

add_executable(${TESTNAME} ${TESTNAME}.cpp)

# needed for some tests
target_compile_options(${TESTNAME} PRIVATE -std=c++20)

target_link_libraries(${TESTNAME}
    gtest gtest_main
    dl
    static_std_exception
    pthread)

target_include_directories(${TESTNAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/lib)

add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})