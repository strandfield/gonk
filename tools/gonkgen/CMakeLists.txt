
##################################################################
###### gonkgen
##################################################################

file(GLOB_RECURSE GONKGEN_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GONKGEN_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(gonkgen ${GONKGEN_HDR_FILES} ${GONKGEN_SRC_FILES})

target_link_libraries(gonkgen libclang-utils)

set_target_properties(gonkgen PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(gonkgen PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
