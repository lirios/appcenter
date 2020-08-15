/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRIAPPCENTERSOFTWAREMANAGER_H
#define LIRIAPPCENTERSOFTWAREMANAGER_H

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SourcesModel>

namespace Liri {

namespace AppCenter {

class SoftwareManagerPrivate;

class LIRIAPPCENTER_EXPORT SoftwareManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SourcesModel *sourcesModel READ sourcesModel CONSTANT)
    Q_PROPERTY(ResourcesModel *resourcesModel READ resourcesModel CONSTANT)
    Q_PROPERTY(bool hasUpdates READ hasUpdates NOTIFY updatesAvailable)
    Q_PROPERTY(uint updatesCount READ updatesCount NOTIFY updatesAvailable)
    Q_DECLARE_PRIVATE(SoftwareManager)
    Q_DISABLE_COPY(SoftwareManager)
public:
    explicit SoftwareManager(QObject *parent = nullptr);
    ~SoftwareManager();

    SourcesModel *sourcesModel() const;
    ResourcesModel *resourcesModel() const;

    Q_INVOKABLE bool addSource(const QString &name);
    Q_INVOKABLE bool removeSource(SoftwareSource *source);

    SoftwareResources updates() const;
    bool hasUpdates() const;
    uint updatesCount() const;

public Q_SLOTS:
    void initialize();
    void checkForUpdates();

Q_SIGNALS:
    void initializationCompleted();
    void sourceAdded(SoftwareSource *source);
    void sourceAddFailed(const QString &name, const QString &errorMessage);
    void updatesAvailable(uint count);

private:
    SoftwareManagerPrivate *const d_ptr;
};

} // namespace AppCenter

} // namesapce Liri

#endif // LIRIAPPCENTERSOFTWAREMANAGER_H
