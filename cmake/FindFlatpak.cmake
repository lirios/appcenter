# SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

find_package(PkgConfig REQUIRED)

pkg_check_modules(Flatpak flatpak>=0.11.8 REQUIRED IMPORTED_TARGET)
