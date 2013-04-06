configure_file(${CMAKE_CURRENT_SOURCE_DIR}/postinstall.sh.in
               ${CMAKE_CURRENT_SOURCE_DIR}/postinstall.sh @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/netinstall.sh.in
               ${CMAKE_CURRENT_SOURCE_DIR}/netinstall.sh @ONLY)

file(GLOB POSTINSTALL_FUNCTIONS
    ${CMAKE_CURRENT_SOURCE_DIR}/postinstall-functions/* )

file(GLOB tribe_SCRIPTS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.sh )

install(FILES ${tribe_SCRIPTS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/scripts
        PERMISSIONS GROUP_READ
                    OWNER_READ
                    WORLD_READ
                    GROUP_EXECUTE
                    OWNER_EXECUTE
                    WORLD_EXECUTE)

install(FILES ${POSTINSTALL_FUNCTIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/scripts)
