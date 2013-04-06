project(tribe)

cmake_minimum_required(VERSION 2.6.0)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(KDE4 REQUIRED)

configure_file(config-tribe.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-tribe.h)

include (MacroLibrary)
include (FindPackageHandleStandardArgs)
include (KDE4Defaults)

find_package(MarbleWidget REQUIRED)
find_package(TribePartitionManager REQUIRED)
find_package(MSGFMT REQUIRED)
find_package(LibPwquality REQUIRED)

add_subdirectory(icons)
add_subdirectory(translations)
add_subdirectory(scripts)
add_subdirectory(data)

add_definitions (
    ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS}
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES}
    ${LIBMARBLEWIDGET_INCLUDE_DIR}
    ${TRIBEPARTITIONMANAGER_INCLUDE_DIR}
)

SET(tribe_SRCS
    src/main.cpp
    src/mainwindow.cpp
    src/installationhandler.cpp
    src/abstractpage.cpp
    src/screenshots.h
    src/pmhandler.cpp
    src/widgets/kbpreview/x11kbparser.cpp
)

SET(tribe_WIDGETS
    src/widgets/progresswidget.cpp
    src/widgets/userwidget.cpp
    src/widgets/avatardialog.cpp
    src/widgets/groupselectionwidget.cpp
    src/widgets/kbpreview/kbpreviewframe.cpp
    src/widgets/kbpreview/keyaliases.cpp
    src/widgets/kbpreview/keyboardlayout.cpp
    src/widgets/kbpreview/keysym2ucs.cpp
    src/widgets/kbpreview/keysymbols.cpp
    src/widgets/kbpreview/keysymhelper.cpp
)

SET(tribe_PAGES
    src/pages/intropage.cpp
    src/pages/releasenotespage.cpp
    src/pages/keyboardpage.cpp
    src/pages/partitionpage.cpp
    src/pages/usercreationpage.cpp
    src/pages/netinstpage.cpp
    src/pages/installationpage.cpp
    src/pages/readyinstallpage.cpp
    src/pages/bootloaderpage.cpp
    src/pages/finishpage.cpp
    src/pages/licensepage.cpp
    src/pages/configpage.cpp
    src/pages/localepage.cpp
)

SET(tribe_PAGE_UIS
    ui/pages/base.ui
    ui/pages/intro.ui
    ui/pages/keyboard.ui
    ui/pages/releasenotes.ui
    ui/pages/license.ui
    ui/pages/locale.ui
    ui/pages/config.ui
    ui/pages/partition.ui
    ui/pages/usercreation.ui
    ui/pages/netinst.ui
    ui/pages/installation.ui
    ui/pages/readyinstall.ui
    ui/pages/bootloader.ui
    ui/pages/finish.ui
)

set(tribe_WIDGET_UIS
    ui/widgets/progresswidget.ui
    ui/widgets/userwidget.ui
    ui/widgets/avatardialog.ui
    ui/widgets/groupselectionwidget.ui
)

SET(tribe_IMAGES
    ui/images/background.svg
    ui/images/sidebar.svg
)

SET(tribe_STYLESHEET
    ui/tribe.qss
)

qt4_add_resources(tribe_RESOUCES ui/tribe.qrc)

kde4_add_ui_files(tribe_SRCS ${tribe_PAGE_UIS} ${tribe_WIDGET_UIS})

kde4_add_executable(tribe
    ${tribe_SRCS}
    ${tribe_PAGES}
    ${tribe_WIDGETS}
    ${tribe_RESOUCES}
)

target_link_libraries(tribe
                      ${KDE4_KDEUI_LIBRARY}
                      ${KDE4_KDECORE_LIBRARY}
                      ${KDE4_KUTILS_LIBRARY}
                      ${KDE4_KIO_LIBRARY}
                      ${LIBMARBLEWIDGET_LIBRARY}
                      ${TRIBEPARTITIONMANAGER_LIBRARY}
                      ${LIBPWQUALITY_LIBRARY}
                      ${KDE4_SOLID_LIBRARY}
                      X11
)

file(GLOB tribe_AVATARS
    ui/images/avatars/*.png )

install(TARGETS tribe DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(FILES ${tribe_AVATARS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/avatars)

install(FILES RELEASE_NOTES.html DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/config)
install(FILES tribe.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

install(FILES ${tribe_STYLESHEET} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/tribe/style)
