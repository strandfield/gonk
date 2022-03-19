
file(GLOB GONK_DEBUGGER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GONK_DEBUGGER_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(gonk-debugger-api STATIC ${GONK_DEBUGGER_SRC_FILES} ${GONK_DEBUGGER_HDR_FILES})
target_include_directories(gonk-debugger-api PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(gonk-debugger-api libgonk libscript)
target_link_libraries(gonk-debugger-api Boost::system)
