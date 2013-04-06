install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/postinstall.conf DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/netinstall.conf  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/mirrorlist       DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cities.kml DESTINATION ${DATA_INSTALL_DIR}/marble/data/placemarks)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/timezones  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/all_locales          DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/all_kde_langpacks    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/configPagePkgData    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/configPageBundleData DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)


file(GLOB appIcons ${CMAKE_CURRENT_SOURCE_DIR}/appIcons/*.png )

install(FILES ${appIcons} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
