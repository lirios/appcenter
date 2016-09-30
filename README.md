App Center
==========

[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-GPLv3%2B-blue.svg)](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
[![GitHub release](https://img.shields.io/github/release/lirios/appcenter.svg)](https://github.com/lirios/appcenter)
[![Build Status](https://travis-ci.org/lirios/appcenter.svg?branch=develop)](https://travis-ci.org/lirios/appcenter)
[![GitHub issues](https://img.shields.io/github/issues/lirios/appcenter.svg)](https://github.com/lirios/appcenter/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2016.svg)](https://github.com/lirios/appcenter/commits/develop)

This is the App Center for qmlOS for installing, updating, and managing applications built using Flatpak.

**_NOTE: This will soon be updated to use QtQuick.Controls 2, Fluid, and Flatpak_**

## Dependencies

Qt >= 5.7.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols](http://code.qt.io/cgit/qt/qtquickcontrols.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
 * [qtgraphicaleffects](http://code.qt.io/cgit/qt/qtgraphicaleffects.git)

The following modules and their dependencies are required:

 * [ECM >= 1.7.0](http://quickgit.kde.org/?p=extra-cmake-modules.git)
 * [fluid](https://github.com/lirios/fluid)
 * [vibe](https://github.com/lirios/vibe)
 * [flatpak](https://github.com/flatpak/flatpak)
 * [karchive](http://quickgit.kde.org/?p=karchive.git)
 * [knotifications](http://quickgit.kde.org/?p=knotifications.git)

## Installation

From the root of the repository, run:

```sh
mkdir build; cd build
cmake .. -DKDE_INSTALL_USE_QT_SYS_PATHS=ON
make
make install # use sudo if necessary
```

On the `cmake` line, you can specify additional configuration parameters:

 * `-DCMAKE_INSTALL_PREFIX=/path/to/install` (for example, `/opt/liri` or `/usr`)
 * `-DCMAKE_BUILD_TYPE=<build_type>`, where `<build_type>` is one of:
   * **Debug:** debug build
   * **Release:** release build
   * **RelWithDebInfo:** release build with debugging information

## Licensing

Licensed under the terms of the GNU General Public License version 3 or,
at your option, any later version.
