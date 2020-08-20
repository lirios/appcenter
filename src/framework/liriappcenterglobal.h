// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERGLOBAL_H
#define LIRIAPPCENTERGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT_BUILD_LIRIAPPCENTER_LIB)
#  define LIRIAPPCENTER_EXPORT Q_DECL_EXPORT
#else
#  define LIRIAPPCENTER_EXPORT Q_DECL_IMPORT
#endif
#define LIRIAPPCENTER_NO_EXPORT Q_DECL_HIDDEN

#endif // LIRIAPPCENTERGLOBAL_H
