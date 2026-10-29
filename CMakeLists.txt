cmake_minimum_required(VERSION 3.16)

project(chinese_chess VERSION 0.1 LANGUAGES CXX)


set(CMAKE_PREFIX_PATH "/opt/Qt/Tools/QtDesignStudio/qt6_design_studio_reduced_version/lib/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Network)



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        chessboard.h chessboard.cpp
        chesspiece.h chesspiece.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chinese_chess
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        welcomewidget.h welcomewidget.cpp
     
    )
else()
    if(ANDROID)
        add_library(chinese_chess SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(chinese_chess
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(chinese_chess PRIVATE Qt${QT_VERSION_MAJOR}::Widgets )
target_link_libraries(chinese_chess PRIVATE Qt6::Concurrent)
target_link_libraries(chinese_chess PRIVATE Qt6::Network)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chinese_chess)
endif()
set_target_properties(chinese_chess PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS chinese_chess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chinese_chess)
endif()
