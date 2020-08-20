// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOFTWARERESOURCE_P_H
#define LIRIAPPCENTERSOFTWARERESOURCE_P_H

#include <QMap>

#include "softwareresource.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Liri {

namespace AppCenter {

class SoftwareResourcePrivate
{
public:
    SoftwareResourcePrivate();

    QMap<QString, QVariant> metadata;
    SoftwareResource::Kudos kudos;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOFTWARERESOURCE_P_H
