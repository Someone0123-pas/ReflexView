# ReflexView

This utility is an adaptation from the original ReflexViewer at https://borokobo.web.fc2.com/ and UraYukimitsu's modification for US-ROMs of Kirby and the Amazing Mirror.
This project is also used as testing ground for some modern C++ features.

Right now, it supports viewing the 30 backgrounds through Qt, or dumping them with the CLI, based on the US-version of the ROM.
This may be extended at a later point.

## Usage
If called without or with one argument, the application will always launch into the Qt GUI.
The first argument must always be the ROM-file. Further arguments can be shown with:
```console
$ reflexview <ROMFILE> -h
```

## Building
Cmake and the Qt libraries must be installed. Then, enter:
```console
$ mkdir build
$ cd build
$ cmake -S .. --preset release
$ cmake --build release
```