cmake_minimum_required(VERSION 3.10)
project(AutoSortDaemon)

enable_testing()

find_package(GTest REQUIRED)


include_directories(${PROJECT_SOURCE_DIR}/include)


add_subdirectory(tests)
