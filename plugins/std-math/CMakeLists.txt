
file(GLOB GONK_STD_MATH_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_STD_MATH_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(std-math SHARED ${GONK_STD_MATH_SRC_FILES} ${GONK_STD_MATH_HDR_FILES})
target_link_libraries(std-math libgonk)
target_compile_definitions(std-math PRIVATE -DGONK_STD_MATH_COMPILE_LIBRARY)

if (WIN32)
  set_target_properties(std-math PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-math")
  set_target_properties(std-math PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/std-math")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-math")
  endforeach()
elseif(UNIX)
  set_target_properties(std-math PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/std-math")
  set_target_properties(std-math PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules/std-math")
  file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/std-math")
endif()
