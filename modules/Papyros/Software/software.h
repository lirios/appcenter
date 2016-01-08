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

#include "xdg-app-plugin.h"

#include <Papyros/QQuickList>

class Application;

class Software: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObjectListModel *remotes READ remotes CONSTANT)
    Q_PROPERTY(QObjectListModel *installedApps READ installedApps CONSTANT)

public:
    Software(QObject *parent = nullptr);

    QObjectListModel *remotes() {
        return m_remotes.getModel();
    }

    QObjectListModel *installedApps() {
        return m_installedApps.getModel();
    }

private slots:
    void update();

private:
    XdgApp *m_xdgApp;
    QQuickList<Remote> m_remotes;
    QQuickList<Application> m_installedApps;
};

#endif // SOFTWARE_H
