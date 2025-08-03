
add_executable(config_tests test_config.cpp)
add_executable(cpuidle_test test_cpuIdle.cpp)

target_link_libraries(config_tests
    ${GTEST_LIB_DIR}/libgtest.a
    ${GTEST_LIB_DIR}/libgtest_main.a
    yaml-cpp
    pthread
)
target_link_libraries(cpuidle_test
    ${GTEST_LIB_DIR}/libgtest.a
    ${GTEST_LIB_DIR}/libgtest_main.a
    ${GTEST_LIB_DIR}/libgmock.a
    ${GTEST_LIB_DIR}/libgmock_main.a
    pthread
)


target_sources(config_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/config/ConfigParser.cpp)
target_sources(cpuidle_test PRIVATE ${PROJECT_SOURCE_DIR}/src/core/CPUIdleMonitor.cpp)

add_test(NAME config_tests COMMAND config_tests)
add_test(NAME cpuidle_tests COMMAND cpuidle_tests)
