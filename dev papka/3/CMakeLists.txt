AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/modspec.cpp")

AC_ADD_SCRIPT_LOADER("modspec" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/modspec.conf.dist")
