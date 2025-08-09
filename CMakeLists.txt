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

link_directories(${GTEST_LIB_DIR}) 

configure_file(${PROJECT_SOURCE_DIR}/config/settings.yaml 
                ${CMAKE_BINARY_DIR}/config/settings.yaml 
                COPYONLY)

add_executable(autosort_daemon

    src/core/CPUIdleMonitor.cpp
    src/core/FileSegregator.cpp
    src/core/FileWatcher.cpp
    src/core/Scheduler.cpp
    
    src/core/utils/Logger.cpp
    
    src/config/ConfigParser.cpp
    
    src/main.cpp
)

target_link_libraries(autosort_daemon
    pthread
    yaml-cpp
)

add_subdirectory(tests)
