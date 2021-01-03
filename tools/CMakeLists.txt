
set(GONK_BUILD_METAGONK OFF CACHE BOOL "whether to build the 'gonk' metagonk tool")

if (GONK_BUILD_METAGONK)
  add_subdirectory(meta)
endif()

set(GONK_BUILD_GONKDBG OFF CACHE BOOL "whether to build the 'gonk' debugger tool")

if (GONK_BUILD_GONKDBG)
  add_subdirectory(gonkdbg)
endif()
