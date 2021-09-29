if( WIN32 )
  if ( MSVC )
    add_custom_command(TARGET worldserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_MODSPEC_DIR}/conf/modspec.conf.dist" ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
    )
  elseif ( MINGW )
    add_custom_command(TARGET worldserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_MODSPEC_DIR}/conf/modspec.conf.dist" ${CMAKE_BINARY_DIR}/bin/
    )
  endif()
endif()

install(FILES "${CMAKE_MODSPEC_DIR}/conf/modspec.conf.dist" DESTINATION ${CONF_DIR})