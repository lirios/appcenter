App Center for qmlOS
====================


[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-LGPLv2.1%2B-blue.svg)](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
[![GitHub release](https://img.shields.io/github/release/qmlos/appcenter.svg)](https://github.com/qmlos/appcenter)
[![Build Status](https://travis-ci.org/qmlos/appcenter.svg?branch=develop)](https://travis-ci.org/qmlos/appcenter)
[![GitHub issues](https://img.shields.io/github/issues/qmlos/appcenter.svg)](https://github.com/qmlos/appcenter/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2016.svg)](https://github.com/qmlos/appcenter/commits/develop)

This is the App Center for qmlOS for installing, updating, and managing applications built using XDG App.

**_NOTE: This will soon be updated to use QtQuick.Controls 2, Fluid, and Flatpak_**

### Dependencies

 * Qt 5.4 or higher
 * QML Material
 * XDG App
 * KDE Frameworks:
   * KArchive
   * KNotifications

### Installation

From the root of the repository, run:

```sh
mkdir build; cd build
cmake .. -DKDE_INSTALL_USE_QT_SYS_PATHS=ON
make
make install # use sudo if necessary
```

On the `cmake` line, you can specify additional configuration parameters:

 * `-DCMAKE_INSTALL_PREFIX=/path/to/install` (for example, `/opt/hawaii` or `/usr`)
 * `-DCMAKE_BUILD_TYPE=<build_type>`, where `<build_type>` is one of:
   * **Debug:** debug build
   * **Release:** release build
   * **RelWithDebInfo:** release build with debugging information

And then run the terminal app:

    qmlos-terminal

### Licensing

qmlOS Terminal is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
