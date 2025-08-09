
add_executable(config_tests test_config.cpp)
add_executable(cpuidle_test test_cpuIdle.cpp)
add_executable(fileseg_test test_fileseg.cpp)
add_executable(scheduler_test test_scheduler.cpp)

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

target_link_libraries(fileseg_test
    ${GTEST_LIB_DIR}/libgtest.a
    ${GTEST_LIB_DIR}/libgtest_main.a
    ${GTEST_LIB_DIR}/libgmock.a       # << Add this line
    ${GTEST_LIB_DIR}/libgmock_main.a  # << And this line
    pthread
)

target_link_libraries(scheduler_test
    ${GTEST_LIB_DIR}/libgtest.a
    ${GTEST_LIB_DIR}/libgtest_main.a
    pthread
)

target_sources(config_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/config/ConfigParser.cpp)
target_sources(cpuidle_test PRIVATE ${PROJECT_SOURCE_DIR}/src/core/CPUIdleMonitor.cpp)
target_sources(fileseg_test PRIVATE ${PROJECT_SOURCE_DIR}/src/core/FileSegregator.cpp
                                    ${PROJECT_SOURCE_DIR}/src/core/utils/Logger.cpp 
                                    )
target_sources(scheduler_test PRIVATE ${PROJECT_SOURCE_DIR}/src/core/Scheduler.cpp
                                      ${PROJECT_SOURCE_DIR}/src/core/CPUIdleMonitor.cpp 
                                      )

add_test(NAME config_tests COMMAND config_tests)
add_test(NAME cpuidle_test COMMAND cpuidle_test)
add_test(NAME fileseg_test COMMAND fileseg_test) 
add_test(NAME scheduler_test COMMAND scheduler_test) 
