/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOFTWARE_H
#define SOFTWARE_H

#include <QObject>

#include <Papyros/QQuickList>

class Application;
class SoftwareSource;
class SoftwareBackend;

class Software : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObjectListModel *sources READ sources CONSTANT)
    Q_PROPERTY(QObjectListModel *availableApps READ availableApps CONSTANT)
    Q_PROPERTY(QObjectListModel *installedApps READ installedApps CONSTANT)

public:
    Software(QObject *parent = nullptr);

    QObjectListModel *sources() { return m_sources.getModel(); }

    QObjectListModel *availableApps() { return m_availableApps.getModel(); }

    QObjectListModel *installedApps() { return m_installedApps.getModel(); }

public slots:
    void downloadUpdates();
    void refreshAvailableApps();

private slots:
    void update();
    void availableApplicationsChanged();

private:
    QList<SoftwareBackend *> m_backends;
    QQuickList<SoftwareSource> m_sources;
    QQuickList<Application> m_availableApps;
    QQuickList<Application> m_installedApps;
};

#endif // SOFTWARE_H
