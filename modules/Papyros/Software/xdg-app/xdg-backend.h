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

#ifndef XDG_BACKEND_H
#define XDG_BACKEND_H

#include "backend.h"

#include "base.h"


class XdgAppBackend: public SoftwareBackend
{
    Q_OBJECT

public:
    XdgAppBackend(QObject *parent = nullptr);

    Q_INVOKABLE QList<Remote *> listRemotes() override;
    Q_INVOKABLE QList<Application *> listInstalledApplications() override;

public slots:
    bool launchApplication(const Application *app) override;

private:
    bool initialize();

    XdgAppInstallation *m_installation = nullptr;
    GFileMonitor *m_monitor = nullptr;
};

#endif // XDG_BACKEND_H
