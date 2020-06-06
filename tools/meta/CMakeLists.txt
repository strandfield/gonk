
##################################################################
###### metagonk
##################################################################

file(GLOB_RECURSE METAGONK_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE METAGONK_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(CMAKE_AUTORCC TRUE)
add_executable(metagonk ${METAGONK_HDR_FILES} ${METAGONK_SRC_FILES} res.qrc)

target_include_directories(metagonk PRIVATE ".")

target_link_libraries(metagonk cxxast)
target_link_libraries(metagonk Qt5::Core Qt5::Widgets Qt5::Sql)

set_target_properties(metagonk PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(metagonk PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

foreach(_source IN ITEMS ${METAGONK_SRC_FILES} ${METAGONK_HDR_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
