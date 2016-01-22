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
#include "software.h"

#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QtConcurrent>

#include "xdg-app/xdg-backend.h"
#include "source.h"
#include "application.h"

Software::Software(QObject *parent) : QObject(parent)
{
    m_backends << new XdgAppBackend(this);

    foreach (SoftwareBackend *backend, m_backends) {
        // TODO: Only update the data from this backend instead of all backends
        QObject::connect(backend, &SoftwareBackend::updated, this, &Software::update);
        QObject::connect(backend, &SoftwareBackend::availableApplicationsChanged, this,
                         &Software::availableApplicationsChanged);
    }

    update();
    availableApplicationsChanged();

    refreshAvailableApps();
}

void Software::downloadUpdates()
{
    // TODO: Run this in the background
    foreach (SoftwareBackend *backend, m_backends) {
        backend->downloadUpdates();
    }
}

void Software::refreshAvailableApps()
{
    QtConcurrent::run([this]() {
        foreach (SoftwareBackend *backend, m_backends) {
            backend->refreshAvailableApplications();
        }
    });
}

void Software::availableApplicationsChanged()
{
    m_availableApps.clear();

    foreach (SoftwareBackend *backend, m_backends) {
        m_availableApps << backend->listAvailableApplications();
    }
}

void Software::update()
{
    // TODO: Update the lists so only new objects are added and old objects removed
    m_sources.clear();
    m_installedApps.clear();

    foreach (SoftwareBackend *backend, m_backends) {
        m_sources << backend->listSources();
        m_installedApps << backend->listInstalledApplications();
    }
}
