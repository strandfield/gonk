
file(GLOB GONK_GONK_DEBUGGER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_GONK_DEBUGGER_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(gonk-debugger SHARED ${GONK_GONK_DEBUGGER_SRC_FILES} ${GONK_GONK_DEBUGGER_HDR_FILES})
target_link_libraries(gonk-debugger gonkbase libscript)
target_link_libraries(gonk-debugger Boost::system)
#target_include_directories(gonk-debugger PUBLIC "${JSONTOOLKIT_INCLUDE_DIRS}")
target_compile_definitions(gonk-debugger PRIVATE -DGONK_GONK_DEBUGGER_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(gonk-debugger PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/gonk-debugger")
  set_target_properties(gonk-debugger PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/gonk-debugger")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/gonk-debugger")
  endforeach()
elseif(UNIX)
  set_target_properties(gonk-debugger PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/gonk-debugger")
  set_target_properties(gonk-debugger PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/gonk-debugger")
  file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/gonk-debugger")
endif()
