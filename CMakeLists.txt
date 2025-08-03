cmake_minimum_required(VERSION 3.10)
project(AutoSortDaemon)

enable_testing()

set(GTEST_DIR ${PROJECT_SOURCE_DIR}/googletest)
set(GTEST_LIB_DIR "${GTEST_DIR}/build/lib")

include_directories(
    ${GTEST_DIR}/googletest/include
    ${GTEST_DIR}/googlemock/include
)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${GTEST_LIB_DIR}) 


add_subdirectory(tests)

