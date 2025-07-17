cmake_minimum_required(VERSION 3.10.0)
project(ReflexView VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Widgets)
qt_standard_project_setup(REQUIRES 6.9)

add_subdirectory(lib/libspng-0.7.4 EXCLUDE_FROM_ALL)
set_target_properties(spng_static PROPERTIES SKIP_AUTOGEN ON)

add_executable(reflexview
    core/main.cpp
    core/byteviewer.cpp
    core/cli.cpp

    core/structviewer/background.cpp
    core/structviewer/palette.cpp
    core/structviewer/roomprops.cpp
    core/structviewer/tileset.cpp
    core/structviewer/tilemap.cpp

    core/algorithms/gbapng.cpp
    core/algorithms/lzss.cpp
    core/algorithms/sha1.cpp

    qt/mainwindow.cpp
    qt/qtui.cpp
    qt/imageprovider.cpp
    qt/debug.cpp
)

set(QMLS
    qt/mainwindow.qml
)

# For flattening resource paths
foreach(qml IN LISTS QMLS)
    get_filename_component(qml_basename "${qml}" NAME)
    set_source_files_properties(${qml} PROPERTIES QT_RESOURCE_ALIAS ${qml_basename})
endforeach()

qt_add_qml_module(reflexview
    URI mainwindow
    RESOURCE_PREFIX "/"
    VERSION 1.0
    QML_FILES
        ${QMLS}
    SOURCES
        include/qmlbridge.h
        qt/qmlbridge.cpp
)

target_link_libraries(reflexview PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    spng_static
)

target_include_directories(reflexview PUBLIC include)
target_compile_options(reflexview PRIVATE
    $<$<CONFIG:debug>:-O1>
)
target_compile_definitions(reflexview PRIVATE
    $<$<CONFIG:debug>:DEBUG>
    $<$<CONFIG:release>:NDEBUG>
)

if(MINGW OR CMAKE_COMPILER_IS_GNUCXX)
target_link_options(reflexview PRIVATE
)
endif()

set_target_properties(reflexview PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS reflexview
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
