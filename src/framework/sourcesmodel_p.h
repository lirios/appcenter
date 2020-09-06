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

#include <QHash>

#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SourcesModel>

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT SourcesModelPrivate
{
    Q_DECLARE_PUBLIC(SourcesModel)
public:
    SourcesModelPrivate() = default;

    static SourcesModelPrivate *get(SourcesModel *q) { return q->d_func(); }

    QHash<int, QByteArray> roles;
    QList<SoftwareSource *> sources;

protected:
    SourcesModel *q_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOURCESMODEL_P_H
