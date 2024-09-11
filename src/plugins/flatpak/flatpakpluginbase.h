// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKPLUGINBASE_H
#define LIRI_FLATPAKPLUGINBASE_H

#undef signals

#ifdef FLATPAK_EXTERNC_REQUIRED
extern "C" {
#endif
#include <flatpak.h>
#ifdef FLATPAK_EXTERNC_REQUIRED
}
#endif

#include <QtCore/qglobal.h>

#endif // LIRI_FLATPAKPLUGINBASE_H
