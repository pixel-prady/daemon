
add_executable(config_tests test_config.cpp)


target_link_libraries(config_tests GTest::GTest GTest::Main yaml-cpp pthread)


target_sources(config_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/config/ConfigParser.cpp)


add_test(NAME config_tests COMMAND config_tests)
