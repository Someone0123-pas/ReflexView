cmake_minimum_required(VERSION 3.10.0)
project(ReflexView VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib/libspng-0.7.4)

add_executable(reflexview)

target_sources(reflexview PRIVATE
    core/main.cpp
    core/byteviewer.cpp
    core/ui_cli.cpp

    core/structviewer/background.cpp
    core/structviewer/palette.cpp
    core/structviewer/roomprops.cpp
    core/structviewer/tileset.cpp

    core/algorithms/gbapng.cpp
    core/algorithms/lzss.cpp
    core/algorithms/sha1.cpp
)

target_include_directories(reflexview PRIVATE include)

target_link_libraries(reflexview PRIVATE spng_static)

target_compile_options(reflexview PRIVATE
    $<$<CONFIG:debug>:-O1>
)

target_compile_definitions(reflexview PRIVATE
    $<$<CONFIG:debug>:DEBUG>
    $<$<CONFIG:release>:NDEBUG>
)
