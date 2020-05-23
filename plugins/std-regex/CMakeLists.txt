
file(GLOB GONK_STD_REGEX_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_STD_REGEX_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(std-regex SHARED ${GONK_STD_REGEX_SRC_FILES} ${GONK_STD_REGEX_HDR_FILES})
target_link_libraries(std-regex libgonk)
target_compile_definitions(std-regex PRIVATE -DGONK_STD_REGEX_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(std-regex PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-regex")
  set_target_properties(std-regex PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-regex")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-regex")
  endforeach()
endif()
