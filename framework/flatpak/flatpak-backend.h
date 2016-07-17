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

#ifndef FLATPAK_BACKEND_H
#define FLATPAK_BACKEND_H

#include "backend.h"

#include <exception>
#include <stdexcept>

#include "base.h"

using namespace std;

struct AppstreamSource
{
    QString origin;
    QString appstreamFilename;
};

class GLibException : public runtime_error
{
public:
    GLibException(const char *what, GError *error) : runtime_error(what), m_error(error) {}

    virtual const char *what() const throw()
    {
        QString error = runtime_error::what();

        if (m_error != nullptr) {
            error = error + ": " + m_error->message;
        }

        return qPrintable(error);
    }

private:
    GError *m_error;
};

class InitializationFailedException : public GLibException
{
public:
    InitializationFailedException(GError *error)
            : GLibException("Unable to initialize Flatpak backend", error)
    {
    }
};

class FlatpakBackend : public SoftwareBackend
{
    Q_OBJECT

public:
    FlatpakBackend(QObject *parent = nullptr);

    QList<SoftwareSource *> listSources() override;
    QList<Application *> listAvailableApplications() override;
    QList<Application *> listInstalledApplications() override;

public slots:
    bool launchApplication(const Application *app) override;
    bool downloadUpdates() override;
    bool refreshAvailableApplications() override;

private:
    void initialize() throw(InitializationFailedException);

    FlatpakInstallation *m_installation = nullptr;
    GFileMonitor *m_monitor = nullptr;
    QList<AppstreamSource> m_appstreamPaths;
};

#endif // FLATPAK_BACKEND_H
