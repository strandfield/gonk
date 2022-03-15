
file(GLOB GONK_PLUGIN_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_PLUGIN_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(gonk-test-hybrid-module SHARED ${GONK_PLUGIN_SRC_FILES} ${GONK_PLUGIN_HDR_FILES})
target_link_libraries(gonk-test-hybrid-module libgonk)
target_compile_definitions(gonk-test-hybrid-module PRIVATE -DGONK_GONKTESTHYBRIDMODULE_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(gonk-test-hybrid-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/gonk-test-hybrid-module")
  set_target_properties(gonk-test-hybrid-module PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/gonk-test-hybrid-module")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/gonk-test-hybrid-module")
    file(COPY "test-hybrid-module.gnk" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/gonk-test-hybrid-module")
  endforeach()
elseif(UNIX)
  set_target_properties(gonk-test-hybrid-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/gonk-test-hybrid-module")
  set_target_properties(gonk-test-hybrid-module PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/gonk-test-hybrid-module")
  file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/gonk-test-hybrid-module")
  file(COPY "test-hybrid-module.gnk" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/gonk-test-hybrid-module")
endif()
