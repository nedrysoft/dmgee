# dmge²

/diː ɛm ʤiː/

---

![Example DMG](https://user-images.githubusercontent.com/55795671/99201460-ef6cc400-27a2-11eb-950b-b70f21b172a6.png)

___

## What is it?

dmge² is a utility that can be used to design and and create DMG files for macOS applications quickly and easily.  It uses python internally to generate the DMG which gives excellent flexibility.

It utiliises opencv to analyse the background image to try to locate the centre points of objects where it believes that an icon (either the application or a symlink to a folder) can be placed, by providing a background image with suitable visual information in, it makes creating a configuration as simple as a couple of clicks.

## Compatibility

Due to the toolset requirements for generating DMG files, this software will only run on macOS.

## Status

The application is work in progress and currently requires a bit more work to be properly usable, but the basic system is in place and it is capable of building disk images.  Some configuration options are not currently linked between the front end and the python backend.

## Installation

Binary distributions can be found under the assets on the [github releases](https://github.com/fizzyade/dmgee/releases) page.

- **Mac OS**.  The application is supplied as a dmg disk image (which itself was built with itself).  Download and open the disk image and drag the dmge² icon into the Applications folder, the application can be launched into design mode by double clicking on the dmge2 icon in Applications folder.

## Requirements (Development)

- cmake for building the application
- python 3 development libraries
- dmgbuild python module
- opencv development libraries
- DevIL development libraries
- toml++ development libraries
- CLI11 development libraries

***The application requires Python 3 to be installed, currently the build does not bundle the python libraries and system modules into the application bundle and relies on them being correctly installed an accessible.***

To build and run the application from source, you will need to ensure that you have python 3 installed and that the modified dmgbuild module is also installed, currently the patch is outstanding while things are finalised, but you can install it directly from my fork with the following command:

```shell script
python3 -m pip install git+https://github.com/fizzyade/dmgbuild.git@master --force
```

## The Ribbon Bar

I have provided a simplified Ribbon Bar widget, this will be moved to it's own repository and switched to a submodule in the near future so that it can be developed indepentently of this or any other projects that make use of it.

It is provided as a dyanmically loaded library and I have added QtDesigner support so that ribbon bars can be created and edited inside designer for convenience.

To build the Ribbon Bar, the following CMake option needs to be on:

```
Build_RibbonDesignerPlugin ON
```

***I recommend creating a symlink from the designer plugin in the binary output folder to the Qt Designer (Or Qt Creator) designer plugins folder, this will allow any changes to the plugin to be made directly avaialble to the IDE (after restarting the IDE) and won't require further copying steps.***

# Credits

The following third party libraries/assets/tools/services have been used in the development of dmge2.

- [cmake](https://www.cmake.org) - cross platform project build system, licensed under [BSD license](https://gitlab.kitware.com/cmake/cmake/raw/master/Copyright.txt).
- [Catch2](https://github.com/catchorg/Catch2) - unit testing framework, licensed under the [BSL-1.0 license](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt).
- [dmgbuild](https://pypi.org/project/dmgbuild/) - Python module for creating DMG files, licensed under the [MIT license](https://github.com/al45tair/dmgbuild/blob/master/LICENSE).
- [opencv](https://opencv.org) - Computer vision and learning libraries, licensed under the [Apache License](https://github.com/opencv/opencv/blob/master/LICENSE).
- [DevIL](http://openil.sourceforge.net) - Image loading library, licensed under the [LGPL-2.1](http://openil.sourceforge.net/license.php).
- [toml](https://github.com/marzer/tomlplusplus) - A toml C++ parser library, licensed under the [MIT license](https://github.com/marzer/tomlplusplus/blob/master/LICENSE)
- [hterm](https://chromium.googlesource.com/apps/libapps/+/master/hterm/) - A javascipt terminal emulator used to proide rich output, licensed under the [BSD license](https://chromium.googlesource.com/apps/libapps/+/HEAD/hterm/LICENSE)

In addition, the following commercially licensed tools/services have also been used.

- [Affinity Designer](https://www.serif.com/designer) - Vector artwork design application.
- [Affinity Photo](https://www.serif.com/photo) - Bitmap artwork design application.
- [CLion](https://www.jetbrains.com/clion/) - C/C++/Obj-C development IDE.
- [Noun Project](https://thenounproject.com) - An icon service with thousands of fantastic icons.  (I have a commercial license)

#  Translations

If any kind souls can provide translations for the application, I will include these and credit you for your work.

Translations can be made using the Qt Linguist tool.

# License

This project is open source and is released under the GPLv3 License

Distributed as-is; no warranty is given.