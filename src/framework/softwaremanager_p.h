// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOFTWAREMANAGER_P_H
#define LIRIAPPCENTERSOFTWAREMANAGER_P_H

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

class BackendPlugin;
class ResourcesModel;
class SourcesModel;

class SoftwareManagerPrivate
{
public:
    SoftwareManagerPrivate();
    ~SoftwareManagerPrivate();

    QVector<Backend *> backends;

    SourcesModel *sourcesModel = nullptr;
    ResourcesModel *resourcesModel = nullptr;
    uint updatesCount = 0;
};

} // namespace AppCenter

} // namesapce Liri

#endif // LIRIAPPCENTERSOFTWAREMANAGER_P_H
