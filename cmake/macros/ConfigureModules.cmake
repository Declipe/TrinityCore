# This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

function(GetModulesBasePath variable)
  set(${variable} "${CMAKE_SOURCE_DIR}/modules" PARENT_SCOPE)
endfunction()

function(GetPathToModuleSource module variable)
  GetModulesBasePath(MODULES_BASE_PATH)
  set(${variable} "${MODULES_BASE_PATH}/${module}/src" PARENT_SCOPE)
endfunction()

function(GetModuleSourceList variable)
  GetModulesBasePath(MODULES_BASE_PATH)
  file(GLOB MODULE_DIRECTORIES CONFIGURE_DEPENDS LIST_DIRECTORIES true "${MODULES_BASE_PATH}/*")

  set(MODULE_LIST)
  foreach(MODULE_DIRECTORY ${MODULE_DIRECTORIES})
    if(IS_DIRECTORY "${MODULE_DIRECTORY}/src")
      get_filename_component(MODULE_NAME "${MODULE_DIRECTORY}" NAME)
      list(APPEND MODULE_LIST ${MODULE_NAME})
    endif()
  endforeach()

  list(SORT MODULE_LIST)
  set(${variable} ${MODULE_LIST} PARENT_SCOPE)
endfunction()

function(ModuleNameToVariable module variable)
  string(TOUPPER "${module}" MODULE_VARIABLE_NAME)
  string(REPLACE "-" "_" MODULE_VARIABLE_NAME "${MODULE_VARIABLE_NAME}")
  set(${variable} "MODULE_${MODULE_VARIABLE_NAME}" PARENT_SCOPE)
endfunction()

function(GetModuleBuildType module variable)
  ModuleNameToVariable("${module}" MODULE_VARIABLE_NAME)
  set(MODULE_BUILD_TYPE "${${MODULE_VARIABLE_NAME}}")

  if(MODULE_BUILD_TYPE STREQUAL "default")
    if(MODULES STREQUAL "static")
      set(MODULE_BUILD_TYPE "static")
    elseif(MODULES STREQUAL "dynamic")
      set(MODULE_BUILD_TYPE "dynamic")
    else()
      set(MODULE_BUILD_TYPE "disabled")
    endif()
  endif()

  set(${variable} "${MODULE_BUILD_TYPE}" PARENT_SCOPE)
endfunction()

function(IsDynamicLinkingModulesRequired variable)
  GetModuleSourceList(MODULE_SOURCE_LIST)
  set(IS_REQUIRED OFF)
  foreach(MODULE ${MODULE_SOURCE_LIST})
    GetModuleBuildType(${MODULE} MODULE_BUILD_TYPE)
    if(MODULE_BUILD_TYPE STREQUAL "dynamic")
      set(IS_REQUIRED ON)
      break()
    endif()
  endforeach()
  set(${variable} ${IS_REQUIRED} PARENT_SCOPE)
endfunction()

macro(AC_ADD_SCRIPT path)
  if(NOT TRINITY_CURRENT_MODULE_NAME)
    message(FATAL_ERROR "AC_ADD_SCRIPT can only be used from a modules/<name>/CMakeLists.txt")
  endif()
  ModuleNameToVariable("${TRINITY_CURRENT_MODULE_NAME}" TRINITY_MODULE_VARIABLE)
  get_filename_component(TRINITY_MODULE_SOURCE "${path}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  set_property(GLOBAL APPEND PROPERTY "${TRINITY_MODULE_VARIABLE}_SOURCES" "${TRINITY_MODULE_SOURCE}")
endmacro()

macro(AC_ADD_SCRIPTS path)
  if(NOT TRINITY_CURRENT_MODULE_NAME)
    message(FATAL_ERROR "AC_ADD_SCRIPTS can only be used from a modules/<name>/CMakeLists.txt")
  endif()
  ModuleNameToVariable("${TRINITY_CURRENT_MODULE_NAME}" TRINITY_MODULE_VARIABLE)
  get_filename_component(TRINITY_MODULE_DIRECTORY "${path}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  file(GLOB_RECURSE TRINITY_MODULE_SOURCES CONFIGURE_DEPENDS
    "${TRINITY_MODULE_DIRECTORY}/*.c"
    "${TRINITY_MODULE_DIRECTORY}/*.cc"
    "${TRINITY_MODULE_DIRECTORY}/*.cpp"
    "${TRINITY_MODULE_DIRECTORY}/*.h"
    "${TRINITY_MODULE_DIRECTORY}/*.hpp")
  set_property(GLOBAL APPEND PROPERTY "${TRINITY_MODULE_VARIABLE}_SOURCES" ${TRINITY_MODULE_SOURCES})
endmacro()

macro(AC_ADD_SCRIPT_LOADER script_dec include)
  if(NOT TRINITY_CURRENT_MODULE_NAME)
    message(FATAL_ERROR "AC_ADD_SCRIPT_LOADER can only be used from a modules/<name>/CMakeLists.txt")
  endif()
  ModuleNameToVariable("${TRINITY_CURRENT_MODULE_NAME}" TRINITY_MODULE_VARIABLE)
  set_property(GLOBAL APPEND PROPERTY "${TRINITY_MODULE_VARIABLE}_LOADERS" "${script_dec}")
endmacro()
