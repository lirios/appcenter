# SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

find_package(PkgConfig)

pkg_check_modules(Flatpak flatpak REQUIRED IMPORTED_TARGET)
