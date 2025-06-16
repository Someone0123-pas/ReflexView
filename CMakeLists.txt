cmake_minimum_required(VERSION 3.10.0)
project(ReflexView VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib/libspng-0.7.4)

add_executable(reflexview)

target_sources(reflexview PRIVATE
    core/main.cpp
    core/algorithms_lzss.cpp
    core/algorithms_png.cpp
    core/algorithms_sha1.cpp
    core/byteviewer.cpp
    core/structviewer.cpp
    core/ui_cli.cpp
)

target_link_libraries(reflexview PRIVATE spng_static)

target_compile_options(reflexview PRIVATE
    -O3 -pedantic -Wall -Wextra
)

target_compile_definitions(reflexview PRIVATE
    $<$<CONFIG:debug>: DEBUG>
)
