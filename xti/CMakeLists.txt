cmake_minimum_required(VERSION 3.24)
# Copyright © Jordan Singh
project(xti VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

set(PROJECT_SOURCES
        main.cpp
        main_window.cpp
        main_window.h
        main_window.ui
        windows_subsystem.h
        windows_subsystem.cpp
        key_mapping.h
        key_mapping.cpp
        error_reporter.h
        error_reporter.cpp
        app_dimensions.h
)
set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/recrypt.rc")
qt_add_executable(xti
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${app_icon_resource_windows}
)
target_link_libraries(xti PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Dwmapi)
target_compile_definitions(xti PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
target_compile_options(xti PRIVATE /EHsc)
target_compile_options(xti PRIVATE /W4 /WX)

set_target_properties(xti PROPERTIES
    MACOSX_BUNDLE_ICON_FILE recrypt.icns
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER net.recrypt.admin
    WIN32_EXECUTABLE TRUE
    AUTOUIC TRUE
    AUTOMOC TRUE
    AUTORCC TRUE
)

include(GNUInstallDirs)
install(TARGETS xti
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(xti)
