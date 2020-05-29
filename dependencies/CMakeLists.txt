
##################################################################
###### libscript
##################################################################

set(LIBSCRIPT_PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/libscript" CACHE PATH "Provide the directory of the libscript library")

if ("${LIBSCRIPT_PROJECT_DIR}" STREQUAL "")
  message(FATAL_ERROR "The directory of the libscript project must be specified")  
endif()

## Create libscript target

file(GLOB_RECURSE LIBSCRIPT_SRC_FILES ${LIBSCRIPT_PROJECT_DIR}/src/*.cpp)
file(GLOB_RECURSE LIBSCRIPT_HDR_FILES 
  ${LIBSCRIPT_PROJECT_DIR}/include/*.h
  ${LIBSCRIPT_PROJECT_DIR}/src/*.h
)

add_library(libscript SHARED ${LIBSCRIPT_HDR_FILES} ${LIBSCRIPT_SRC_FILES})
target_include_directories(libscript PUBLIC "${LIBSCRIPT_PROJECT_DIR}/include")
target_compile_definitions(libscript PRIVATE -DLIBSCRIPT_COMPILE_LIBRARY)
#target_compile_definitions(libscript PUBLIC -DLIBSCRIPT_STATIC_LINKING)

foreach(_source IN ITEMS ${LIBSCRIPT_HDR_FILES} ${LIBSCRIPT_SRC_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

if (WIN32)
  set_target_properties(libscript PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(libscript PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

##################################################################
###### cxxast
##################################################################

add_subdirectory(cxxast)
