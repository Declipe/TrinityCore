AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/ModAutoLearn.cpp")

AC_ADD_SCRIPT_LOADER("ModAutoLearn" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/ModAutoLearn.conf.dist")
