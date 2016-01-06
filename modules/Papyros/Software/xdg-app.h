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

class XdgApp: public QObject
{
    Q_OBJECT

public:
    XdgApp(QObject *parent = nullptr);

    // TODO: Make these objects return some sort of background transaction
    Q_INVOKABLE void addRemote(QString repoName, QString url) const;
    Q_INVOKABLE void installApp(QString remoteName, QString appName) const;
    Q_INVOKABLE void updateApp(QString appName) const;
    Q_INVOKABLE void installRuntime(QString remoteName, QString runtimeName) const;
    Q_INVOKABLE QStringList listApps() const;
};

#endif // XDG_APP_H
