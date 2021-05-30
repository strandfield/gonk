
##################################################################
###### gonkdbg
##################################################################

# Copy icon
file(COPY ${CMAKE_SOURCE_DIR}/icon.ico ${CMAKE_SOURCE_DIR}/icon.svg DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE GONKDBG_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GONKDBG_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(GONKDBG_RESOURCES "res.qrc")

IF (WIN32)
  set(GONKDBG_RESOURCES "windows_res.rc" ${GONKDBG_RESOURCES})
ENDIF()

set(CMAKE_AUTORCC TRUE)
add_executable(gonkdbg ${GONKDBG_HDR_FILES} ${GONKDBG_SRC_FILES} ${GONKDBG_RESOURCES})

target_include_directories(gonkdbg PRIVATE ".")

target_link_libraries(gonkdbg gonk-debugger-api)
target_link_libraries(gonkdbg Qt5::Core Qt5::Widgets Qt5::Network)

set_target_properties(gonkdbg PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(gonkdbg PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

foreach(_source IN ITEMS ${GONKDBG_SRC_FILES} ${GONKDBG_HDR_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

if (WIN32)
  set_target_properties(gonkdbg PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_DIR}/../../../bin;%PATH%")
endif()

