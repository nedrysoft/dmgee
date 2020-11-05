# dmgee

/diː ɛm ʤiː/

dmgee is a utility that can be used to design and and create DMG files for macOS applications quickly and easily.  It uses python internally to generate the DMG which gives excellent flexibility.

It utiliises opencv to analyse the background image to try to locate the centre points of objects where it believes that an icon (either the application or a symlink to a folder) can be placed, by providing a background image with suitable visual information in, it makes creating a configuration as simple as a couple of clicks.

## Compatibility

Due to the toolset requirements for generating DMG files, this software will only run on macOS.

## Installation

Binary distributions can be found under the assets on the [github releases](https://github.com/fizzyade/dmgee/releases) page.

- **Mac OS**.  The application is supplied as a dmg disk image (which itself was built with itself).  Download and open the disk image and drag the dmgee icon into the Applications folder, the application can be launched into design mode by double clicking on the dmgee icon in Applications folder.

## Requirements (Development)

- cmake for building the application
- python 3 development libraries
- dmgbuild python module
- opencv development libraries
- DevIL development libraries
- toml++ development libraries
- CLI11 development libraries

# Credits

The following third party libraries/assets/tools/services have been used in the development of dmgee.

- [cmake](https://www.cmake.org) - cross platform project build system, licensed under [BSD license](https://gitlab.kitware.com/cmake/cmake/raw/master/Copyright.txt).
- [Catch2](https://github.com/catchorg/Catch2) - unit testing framework, licensed under the [BSL-1.0 license](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt).
- [dmgbuild](https://pypi.org/project/dmgbuild/) - Python module for creating DMG files, licensed under the [MIT license](https://github.com/al45tair/dmgbuild/blob/master/LICENSE).
- [opencv](https://opencv.org) - Computer vision and learning libraries, licensed under the [Apache License](https://github.com/opencv/opencv/blob/master/LICENSE).
- [DevIL](http://openil.sourceforge.net) - Image loading library, licensed under the [LGPL-2.1](http://openil.sourceforge.net/license.php).
- [toml](https://github.com/marzer/tomlplusplus) - A toml C++ parser library, licensed under the [MIT license](https://github.com/marzer/tomlplusplus/blob/master/LICENSE)

In addition, the following commercially licensed tools/services have also been used.

- [Affinity Designer](https://www.serif.com/designer) - Vector artwork design application.
- [Affinity Photo](https://www.serif.com/photo) - Bitmap artwork design application.
- [CLion](https://www.jetbrains.com/clion/) - C/C++/Obj-C development IDE.

# License

This project is open source and is released under the GPLv3 License

Distributed as-is; no warranty is given.