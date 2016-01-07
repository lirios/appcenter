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

#ifndef XDG_APP_H
#define XDG_APP_H

#include <QObject>
#include <QString>

#include "base.h"

class Application;
class Remote;

class XdgApp: public QObject
{
    Q_OBJECT

public:
    XdgApp(QObject *parent = nullptr);

    Q_INVOKABLE QList<Remote *> listRemotes();
    Q_INVOKABLE QList<Application *> listInstalledApplications();

signals:
    void installationChanged();

private:
    bool initialize();

    XdgAppInstallation *m_installation = nullptr;
    GFileMonitor *m_monitor = nullptr;
};

#endif // XDG_APP_H
