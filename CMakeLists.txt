cmake_minimum_required(VERSION 3.9)
project(gonk)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets)

##################################################################
###### dependencies
##################################################################

add_subdirectory(dependencies)

##################################################################
###### gonk
##################################################################

file(GLOB_RECURSE GONK_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE GONK_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/gonk/*.h)

list(REMOVE_ITEM GONK_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(libgonk SHARED ${GONK_SRC_FILES} ${GONK_HDR_FILES})
target_include_directories(libgonk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(libgonk Qt5::Core)
target_link_libraries(libgonk libscript)
target_compile_definitions(libgonk PRIVATE -DGONK_COMPILE_LIBRARY)

add_executable(gonk 
               "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(gonk libgonk)

foreach(_source IN ITEMS ${GONK_SRC_FILES} ${GONK_HDR_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

if (WIN32)
  set_target_properties(gonk PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(gonk PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

##################################################################
###### plugins
##################################################################

add_subdirectory(plugins)

##################################################################
###### Tests, tools & examples
##################################################################

#add_subdirectory(tests)
#add_subdirectory(tools)
#add_subdirectory(examples)

# Copy examples to build directory
file(GLOB_RECURSE GONK_EXAMPLES_FILES "examples/*.gnk")
file(COPY ${GONK_EXAMPLES_FILES} DESTINATION ${CMAKE_BINARY_DIR})
