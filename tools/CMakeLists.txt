
set(GONK_BUILD_GONKDBG OFF CACHE BOOL "whether to build the 'gonk' debugger tool")

if (GONK_BUILD_GONKDBG)
  add_subdirectory(gonkdbg)
endif()

set(GONK_BUILD_GONKGEN OFF CACHE BOOL "whether to build the 'gonkgen' tool")

if (GONK_BUILD_GONKGEN)
  add_subdirectory(gonkgen)
endif()
