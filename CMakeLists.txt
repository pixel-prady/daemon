cmake_minimum_required(VERSION 3.10)
project(AutoSortDaemon)

enable_testing()

set(GTEST_DIR ${PROJECT_SOURCE_DIR}/googletest)
set(GTEST_LIB_DIR "${GTEST_DIR}/build/lib")

include_directories(
    ${PROJECT_SOURCE_DIR}/include         
    ${PROJECT_SOURCE_DIR}/include/core    
    ${PROJECT_SOURCE_DIR}/include/core/utils 
    ${PROJECT_SOURCE_DIR}/include/config 
    ${PROJECT_SOURCE_DIR}/googletest/googletest/include  
    ${PROJECT_SOURCE_DIR}/googletest/googlemock/include  
    ${PROJECT_SOURCE_DIR}/tests          
)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${GTEST_LIB_DIR}) 

add_subdirectory(tests)
