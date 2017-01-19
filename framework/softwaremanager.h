/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef SOFTWARE_H
#define SOFTWARE_H

#include <QObject>

#include <Vibe/Core/QQuickList>

#include <software/software_export.h>

class Application;
class SoftwareSource;
class SoftwareBackend;

class SOFTWARE_EXPORT SoftwareManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObjectListModel *sources READ sources CONSTANT)
    Q_PROPERTY(QObjectListModel *availableApps READ availableApps CONSTANT)
    Q_PROPERTY(QObjectListModel *availableUpdates READ availableUpdates CONSTANT)
    Q_PROPERTY(QObjectListModel *installedApps READ installedApps CONSTANT)
    Q_PROPERTY(bool hasUpdates READ hasUpdates NOTIFY updated)
    Q_PROPERTY(QString updatesSummary READ updatesSummary NOTIFY updated)

public:
    SoftwareManager(QObject *parent = nullptr);

    QObjectListModel *sources() { return m_sources.getModel(); }

    QObjectListModel *availableApps() { return m_availableApps.getModel(); }
    QObjectListModel *availableUpdates() { return m_availableUpdates.getModel(); }

    QObjectListModel *installedApps() { return m_installedApps.getModel(); }

    bool hasUpdates() const { return m_availableUpdates.count() > 0; }

    QString updatesSummary() const;

public Q_SLOTS:
    void refresh();
    void downloadUpdates();
    void refreshAvailableApps();

private Q_SLOTS:
    void update();
    void availableApplicationsChanged();

Q_SIGNALS:
    void updatesDownloaded();
    void updated();

private:
    QList<SoftwareBackend *> m_backends;
    QQuickList<SoftwareSource> m_sources;
    QQuickList<Application> m_availableApps;
    QQuickList<Application> m_availableUpdates;
    QQuickList<Application> m_installedApps;
};

#endif // SOFTWARE_H
