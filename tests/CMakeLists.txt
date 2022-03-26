
enable_testing()

if(NOT DEFINED GONK_DOWNLOADED_CATCH2 OR NOT GONK_DOWNLOADED_CATCH2)

  message("Downloading Catch2")
  file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.13.8/catch.hpp "${CMAKE_CURRENT_LIST_DIR}/catch.hpp" SHOW_PROGRESS)
  
  set(GONK_DOWNLOADED_CATCH2 TRUE CACHE BOOL "Whether Catch2 was downloaded")

endif()

## Unit tests

add_executable(TEST_gonk_unit_tests unittests.cpp "${CMAKE_CURRENT_SOURCE_DIR}/catch.hpp")
target_link_libraries(TEST_gonk_unit_tests gonkbase)

if (WIN32)
  set_target_properties(TEST_gonk_unit_tests PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(TEST_gonk_unit_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

add_test(NAME "TEST_gonk_unit_tests" COMMAND TEST_gonk_unit_tests)

## Test suite

configure_file("gonk-test-resources.h.in" "gonk-test-resources.h")

add_executable(TEST_gonk_test_suite testsuite.cpp "${CMAKE_CURRENT_BINARY_DIR}/gonk-test-resources.h")
target_include_directories(TEST_gonk_test_suite PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(TEST_gonk_test_suite gonkbase)

if (WIN32)
  set_target_properties(TEST_gonk_test_suite PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(TEST_gonk_test_suite PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

add_test(NAME "TEST_gonk_test_suite" COMMAND TEST_gonk_test_suite)
