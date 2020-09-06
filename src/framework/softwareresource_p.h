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

#include <LiriAppCenter/Rating>
#include <LiriAppCenter/SoftwareManager>

namespace Liri {

namespace AppCenter {

class SoftwareSource;

class SoftwareResourcePrivate
{
    Q_DECLARE_PUBLIC(SoftwareResource)
public:
    SoftwareResourcePrivate(SoftwareResource *self);

    void setRating(Rating *r);

    static SoftwareResourcePrivate *get(SoftwareResource *res) { return res->d_func(); }

    SoftwareManager *manager = nullptr;
    SoftwareSource *source = nullptr;
    QMap<QString, QVariant> metadata;
    SoftwareResource::Kudos kudos;
    Rating *rating = nullptr;
    QList<Review *> reviews;

protected:
    SoftwareResource *q_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOFTWARERESOURCE_P_H
