// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_RESOURCEPROXY_P_H
#define LIRI_APPCENTER_RESOURCEPROXY_P_H

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

#include <LiriAppCenter/ResourceProxy>
#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SourcesModel>

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT ResourceProxyPrivate
{
public:
    ResourceProxyPrivate();
    ~ResourceProxyPrivate();

    QString id;
    SoftwareSource *defaultSource = nullptr;
    SoftwareResource *selectedResource = nullptr;
    QList<SoftwareResource *> resources;
    SourcesModel *sourcesModel = nullptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_RESOURCEPROXY_P_H
