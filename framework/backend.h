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

#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class Application;
class SoftwareSource;

class SoftwareBackend : public QObject
{
    Q_OBJECT

public:
    SoftwareBackend(QObject *parent = nullptr) : QObject(parent) {}

    virtual QList<SoftwareSource *> listSources() = 0;
    virtual QList<Application *> listInstalledApplications() = 0;
    virtual QList<Application *> listAvailableApplications() = 0;

public slots:
    virtual bool launchApplication(const Application *app) = 0;
    virtual bool downloadUpdates() = 0;
    virtual bool refreshAvailableApplications() = 0;

signals:
    void updated();
    void availableApplicationsChanged();
};

#endif // BACKEND_H
