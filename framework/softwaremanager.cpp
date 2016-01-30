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
#include "softwaremanager.h"

#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QtConcurrent>

#include "xdg-app/xdg-backend.h"
#include "source.h"
#include "application.h"

SoftwareManager::SoftwareManager(QObject *parent) : QObject(parent)
{
    m_backends << new XdgAppBackend(this);

    foreach (SoftwareBackend *backend, m_backends) {
        // TODO: Only update the data from this backend instead of all backends
        QObject::connect(backend, &SoftwareBackend::updated, this, &SoftwareManager::update);
        QObject::connect(backend, &SoftwareBackend::availableApplicationsChanged, this,
                         &SoftwareManager::availableApplicationsChanged);
    }

    QObject::connect(this, &SoftwareManager::updatesDownloaded, this, &SoftwareManager::update);
}

void SoftwareManager::refresh()
{
    update();
    availableApplicationsChanged();

    refreshAvailableApps();
}

void SoftwareManager::downloadUpdates()
{
    QtConcurrent::run([this]() {
        foreach (SoftwareBackend *backend, m_backends) {
            backend->downloadUpdates();
        }

        emit updatesDownloaded();
    });
}

void SoftwareManager::refreshAvailableApps()
{
    QtConcurrent::run([this]() {
        foreach (SoftwareBackend *backend, m_backends) {
            backend->refreshAvailableApplications();
        }
    });
}

void SoftwareManager::availableApplicationsChanged()
{
    m_availableApps.clear();

    foreach (SoftwareBackend *backend, m_backends) {
        m_availableApps << backend->listAvailableApplications();
    }

    emit updated();
}

void SoftwareManager::update()
{
    // TODO: Update the lists so only new objects are added and old objects removed
    m_sources.clear();
    m_installedApps.clear();
    m_availableUpdates.clear();

    foreach (SoftwareBackend *backend, m_backends) {
        m_sources << backend->listSources();
        m_installedApps << backend->listInstalledApplications();
    }

    QList<Application *> apps = m_installedApps;

    foreach (Application *app, apps) {
        if (app->updatesAvailable() && app->m_type == Application::App)
            m_availableUpdates << app;
    }

    emit updated();
}

QString SoftwareManager::updatesSummary() const
{
    if (m_availableUpdates.count() == 0) {
        return tr("No updates are available");
    } else if (m_availableUpdates.count() == 1) {
        return tr("%1 is ready to update").arg(m_availableUpdates[0]->m_name);
    } else if (m_availableUpdates.count() == 2) {
        return tr("%1 and %2 are ready to update")
                .arg(m_availableUpdates[0]->m_name)
                .arg(m_availableUpdates[1]->m_name);
    } else if (m_availableUpdates.count() == 3) {
        return tr("%1, %2, and one other app are ready to update")
                .arg(m_availableUpdates[0]->m_name)
                .arg(m_availableUpdates[1]->m_name);
    } else {
        int otherCount = m_availableUpdates.count() - 2;

        return tr("%1, %2, and %3 other apps are ready to update")
                .arg(m_availableUpdates[0]->m_name)
                .arg(m_availableUpdates[1]->m_name)
                .arg(otherCount);
    }
}
