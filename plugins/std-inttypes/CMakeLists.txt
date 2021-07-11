
file(GLOB GONK_STD_INTTYPES_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_STD_INTTYPES_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(std-inttypes SHARED ${GONK_STD_INTTYPES_SRC_FILES} ${GONK_STD_INTTYPES_HDR_FILES})
target_link_libraries(std-inttypes libgonk)
target_compile_definitions(std-inttypes PRIVATE -DGONK_STD_INTTYPES_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(std-inttypes PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-inttypes")
  set_target_properties(std-inttypes PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-inttypes")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-inttypes")
  endforeach()
elseif(UNIX)
  set_target_properties(std-inttypes PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/std-inttypes")
  set_target_properties(std-inttypes PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/std-inttypes")
  file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-inttypes")
endif()
