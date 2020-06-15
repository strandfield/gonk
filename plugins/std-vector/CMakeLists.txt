
file(GLOB GONK_STD_VECTOR_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_STD_VECTOR_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(std-vector SHARED ${GONK_STD_VECTOR_SRC_FILES} ${GONK_STD_VECTOR_HDR_FILES})
target_link_libraries(std-vector libgonk)
target_compile_definitions(std-vector PRIVATE -DGONK_STD_VECTOR_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(std-vector PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-vector")
  set_target_properties(std-vector PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-vector")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-vector")
  endforeach()
endif()
