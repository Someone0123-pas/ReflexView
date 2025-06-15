cmake_minimum_required(VERSION 3.10.0)
project(ReflexView VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

set(CORESRCS
    core/main.cpp
    core/algorithms_lzss.cpp
    core/algorithms_png.cpp
    core/algorithms_sha1.cpp
    core/byteviewer.cpp
    core/structviewer.cpp
    core/ui_cli.cpp
)

add_executable(reflexview ${CORESRCS})
target_compile_options(reflexview PRIVATE
    -O3 -pedantic -Wall -Wextra
    $<$<CONFIG:debug>: -O0>
)
target_compile_definitions(reflexview PRIVATE
    $<$<CONFIG:debug>: DEBUG>
)
