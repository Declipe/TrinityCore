# Load optional per-module CMake configuration before compiling the server scripts.
GetModuleSourceList(MODULE_SOURCE_LIST)

foreach(SOURCE_MODULE ${MODULE_SOURCE_LIST})
  GetModuleBuildType(${SOURCE_MODULE} MODULE_BUILD_TYPE)
  if(MODULE_BUILD_TYPE STREQUAL "disabled")
    continue()
  endif()

  set(TRINITY_CURRENT_MODULE_NAME "${SOURCE_MODULE}")
  set(MODULE_CMAKE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_MODULE}/CMakeLists.txt")
  if(EXISTS "${MODULE_CMAKE_FILE}")
    add_subdirectory("${SOURCE_MODULE}" "${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_MODULE}")
  endif()
endforeach()
