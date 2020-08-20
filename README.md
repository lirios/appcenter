<!--
SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>

SPDX-License-Identifier: CC0-1.0
-->

App Center
==========

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/appcenter.svg)](https://github.com/lirios/appcenter)
[![GitHub issues](https://img.shields.io/github/issues/lirios/appcenter.svg)](https://github.com/lirios/appcenter/issues)
[![CI](https://github.com/lirios/appcenter/workflows/CI/badge.svg?branch=develop&event=push)](https://github.com/lirios/appcenter/actions?query=workflow%3ACI)
[![REUSE status](https://api.reuse.software/badge/github.com/lirios/appcenter)](https://api.reuse.software/info/github.com/lirios/appcenter)

This is the App Center for Liri OS for installing, updating, and managing applications built using Flatpak.

## Dependencies

Qt >= 5.10.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

The following modules and their dependencies are required:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0
 * [qtaccountsservice](https://github.com/lirios/qtaccountsservice.git) >= 1.2.0
 * [fluid](https://github.com/lirios/fluid) >= 1.0.0
 * [libliri](https://github.com/lirios/libliri)
 * [flatpak](https://github.com/flatpak/flatpak)
 * [AppStream](https://github.com/ximion/appstream)

## Installation

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/prefix ..
make
make install # use sudo if necessary
```

Replace `/path/to/prefix` to your installation prefix.
Default is `/usr/local`.

### Logging categories

Qt 5.2 introduced logging categories and we take advantage of
them to make debugging easier.

Please refer to the [Qt](http://doc.qt.io/qt-5/qloggingcategory.html) documentation
to learn how to enable them.

### Available categories

* **liri.appcenter:** Library and application
* **liri.appcenter.flatpak:** Flatpak backend
* **liri.appcenter.odrs:** ODRS reviews backend

## Licensing

Licensed under the terms of the GNU General Public License version 3 or,
at your option, any later version.
