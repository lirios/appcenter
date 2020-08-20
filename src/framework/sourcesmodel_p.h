// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOURCESMODEL_P_H
#define LIRIAPPCENTERSOURCESMODEL_P_H

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

class SourcesModelPrivate
{
public:
    SourcesModelPrivate();
    ~SourcesModelPrivate();

    QHash<int, QByteArray> roles;
    QVector<SoftwareSource *> sources;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOURCESMODEL_P_H
