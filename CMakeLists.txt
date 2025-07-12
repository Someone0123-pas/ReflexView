cmake_minimum_required(VERSION 3.10.0)
project(ReflexView VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.9)

add_executable(reflexview
    core/main.cpp
    core/byteviewer.cpp
    core/ui_cli.cpp
    core/ui_qt.cpp

    core/structviewer/background.cpp
    core/structviewer/palette.cpp
    core/structviewer/roomprops.cpp
    core/structviewer/tileset.cpp
    core/structviewer/tilemap.cpp

    core/algorithms/gbapng.cpp
    core/algorithms/lzss.cpp
    core/algorithms/sha1.cpp

    qt/mainwindow.cpp
    qt/debug.cpp
    qt/imageprovider.cpp
)

set(QMLS
    qt/mainwindow.qml
)

foreach(qml IN LISTS QMLS)
    get_filename_component(qml_basename "${qml}" NAME)
    set_source_files_properties(${qml} PROPERTIES QT_RESOURCE_ALIAS ${qml_basename})
endforeach()

qt_add_qml_module(reflexview
    URI mainwindow
    RESOURCE_PREFIX "/"
    QML_FILES
        ${QMLS}
)

add_subdirectory(lib/libspng-0.7.4 EXCLUDE_FROM_ALL)
set_target_properties(spng_static PROPERTIES SKIP_AUTOGEN ON)

target_link_libraries(reflexview PRIVATE
    Qt6::Quick
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
