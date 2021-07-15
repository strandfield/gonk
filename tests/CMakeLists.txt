
enable_testing()

add_executable(TEST_gonk_binding_utils test.cpp)
target_link_libraries(TEST_gonk_binding_utils libgonk)

if (WIN32)
  set_target_properties(TEST_gonk_binding_utils PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(TEST_gonk_binding_utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  set_target_properties(TEST_gonk_binding_utils PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_DIR}/../../../bin;%PATH%")
endif()

add_test(NAME "TEST_gonk_binding_utils" COMMAND TEST_gonk_binding_utils)
