cmake_minimum_required(VERSION 3.10.0)
project(ReflexView VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)
add_subdirectory(lib/libspng-0.7.4 EXCLUDE_FROM_ALL)

qt_standard_project_setup()

qt_add_executable(reflexview
    core/main.cpp
    core/byteviewer.cpp
    core/ui_cli.cpp

    core/structviewer/background.cpp
    core/structviewer/palette.cpp
    core/structviewer/roomprops.cpp
    core/structviewer/tileset.cpp
    core/structviewer/tilemap.cpp

    core/algorithms/gbapng.cpp
    core/algorithms/lzss.cpp
    core/algorithms/sha1.cpp

    qt/init.cpp
)

qt_add_resources(reflexview
    PREFIX "/"
    BASE "qt"
    FILES
        qt/init.qml
)

target_link_libraries(reflexview PRIVATE
    Qt6::Quick
    spng_static
)

set_target_properties(spng_static PROPERTIES AUTOMOC OFF)

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
