# dmge²

/diː ɛm ʤiː/

---

![Example DMG](https://bit.ly/33i5lKE)

___

## What is it?

dmge² is a utility to design and create DMG files for macOS applications quickly and easily.  It uses python internally to generate the DMG, which gives excellent flexibility.

OpenCV is optionally used to analyse the background image to locate the centre points of objects where.   A background image with relevant visual information makes designing even more effortless.

## Compatibility

Due to the toolset requirements for generating DMG files, this software will only run on macOS.

## Status

The application is currently a "work in progress", but the basic architecture is in place, and it is capable of building disk images.

## Installation

Binary distributions are available from the releases on the [github releases](https://github.com/fizzyade/dmgee/releases) page.

- **Mac OS**.  Download and open the disk image and drag the dmge² icon into the Applications folder, you can then start in design mode by double-clicking on the dmge2 icon in the Applications folder.

## Requirements (Development)

- CMake for building the application
- python 3 development libraries
- dmgbuild python module
- OpenCV development libraries
- DevIL development libraries
- toml++ development libraries
- CLI11 development libraries
- yaml-cpp development libraries

***The application requires that the user has Python 3 installed; currently, the build does not bundle the python libraries and system modules into the application bundle and relies on them being correctly installed and accessible.***

To build and run the application from source, you will need to ensure that you have Python 3 installed and the modified dmgbuild module.  Whilst things are finalised, but you can install it directly from my fork with the following command:

```shell script
python3 -m pip install git+https://github.com/fizzyade/dmgbuild.git@master --force
```

## The Ribbon Bar

I will move the Ribbon Bar widget library to its repository once the application has reached its first release.  

The Ribbon Bar is a dynamically loaded library, and I have added QtDesigner support so that ribbon bars can be created and edited inside designer for convenience.

By default, the Designer plugin is disabled; to enable building use the following CMake option.

```
Build_RibbonDesignerPlugin ON
```

***I recommend creating a symlink from the designer plugin in the binary output folder to the Qt Designer (Or Qt Creator) designer plugins folder.  By doing this, any changes to the plugin will be available to the Designer or Qt Creator (after restarting the application) and will not require further copying steps for installation.***

# Credits

Third-party libraries/assets/tools/services were used in the development of dmge2.

- [cmake](https://www.cmake.org) - cross platform project build system, licensed under [BSD license](https://gitlab.kitware.com/cmake/cmake/raw/master/Copyright.txt).
- [Catch2](https://github.com/catchorg/Catch2) - unit testing framework, licensed under the [BSL-1.0 license](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt).
- [dmgbuild](https://pypi.org/project/dmgbuild/) - Python module for creating DMG files, licensed under the [MIT license](https://github.com/al45tair/dmgbuild/blob/master/LICENSE).
- [opencv](https://opencv.org) - Computer vision and learning libraries, licensed under the [Apache License](https://github.com/opencv/opencv/blob/master/LICENSE).
- [DevIL](http://openil.sourceforge.net) - Image loading library, licensed under the [LGPL-2.1](http://openil.sourceforge.net/license.php).
- [toml](https://github.com/marzer/tomlplusplus) - A toml C++ parser library, licensed under the [MIT license](https://github.com/marzer/tomlplusplus/blob/master/LICENSE)
- [hterm](https://chromium.googlesource.com/apps/libapps/+/master/hterm/) - A javascipt terminal emulator used to proide rich output, licensed under the [BSD license](https://chromium.googlesource.com/apps/libapps/+/HEAD/hterm/LICENSE)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) - A YAML parser and emitter used to decode the metadata in the licence files, licensed under the [MIT license](https://raw.githubusercontent.com/jbeder/yaml-cpp/master/LICENSE)
- [choosealicense.com](https://github.com/github/choosealicense.com) - Provides a repository of open source licenses which contain metadata to show extra information, licensed under the [Creative Commons Attribution 3.0 Unported license](https://creativecommons.org/licenses/by/3.0/)
- [spdx.org](https://spdx.org) - The official repository of SPDX licenses, licensed under the [PDDL-1.0 license](https://opendatacommons.org/licenses/pddl/1-0/)

Other propriety or licensed tools/services used during development.

- [Affinity Designer](https://www.serif.com/designer) - Vector artwork design application.
- [Affinity Photo](https://www.serif.com/photo) - Bitmap artwork design application.
- [CLion](https://www.jetbrains.com/clion/) - C/C++/Obj-C development IDE.
- [Noun Project](https://thenounproject.com) - An icon service with thousands of fantastic icons.  (I have a commercial license)

#  Translations

Translations of the software are welcome, and you can supply these via GitHub with a pull request.

To create translations, you should use Qt Linguist.

# License

This project is open source and released under the GPLv3 licence.

Distributed as-is; no warranty is given.
