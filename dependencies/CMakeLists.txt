
##################################################################
###### libscript
##################################################################

set(LIBSCRIPT_BUILD_TESTS OFF CACHE BOOL "whether to build the libscript tests")
set(LIBSCRIPT_BUILD_EXAMPLES OFF CACHE BOOL "whether to build the libscript examples")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(libscript)

set_target_properties(libscript PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(libscript PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

##################################################################
###### cxxast
##################################################################

add_subdirectory(cxxast)

##################################################################
###### liquid
##################################################################

add_subdirectory(liquid)

if (WIN32)
  set_target_properties(liquid PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(liquid PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()
