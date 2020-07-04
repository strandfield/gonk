
file(GLOB GONK_QT_CORE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_QT_CORE_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(qt-core SHARED ${GONK_QT_CORE_SRC_FILES} ${GONK_QT_CORE_HDR_FILES})
target_link_libraries(qt-core libgonk)
target_link_libraries(qt-core Qt5::Core)
target_compile_definitions(qt-core PRIVATE -DGONK_QT_CORE_COMPILE_LIBRARY)
target_include_directories(qt-core PRIVATE "${CMAKE_CURRENT_LIST_DIR}/..")

if (WIN32)
  set_target_properties(qt-core PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/qt-core")
  set_target_properties(qt-core PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/modules/qt-core")

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    file(COPY "gonkmodule" DESTINATION "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/modules/qt-core")
  endforeach()
endif()
