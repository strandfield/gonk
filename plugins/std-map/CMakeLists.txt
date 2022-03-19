
file(GLOB GONK_PLUGIN_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_PLUGIN_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(std-map SHARED ${GONK_PLUGIN_SRC_FILES} ${GONK_PLUGIN_HDR_FILES})
target_link_libraries(std-map gonkbase)
target_compile_definitions(std-map PRIVATE -DGONK_STD_MAP_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(std-map PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-map")
  set_target_properties(std-map PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-map")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-map")
  endforeach()
elseif(UNIX)
  set_target_properties(std-map PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/std-map")
  set_target_properties(std-map PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/std-map")
  file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-map")
endif()
