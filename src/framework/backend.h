/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRIAPPCENTERBACKEND_H
#define LIRIAPPCENTERBACKEND_H

#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SoftwareSource>

namespace Liri {

namespace AppCenter {

class SoftwareManager;

class LIRIAPPCENTER_EXPORT Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    virtual ~Backend();

    virtual void initialize(SoftwareManager *manager) = 0;

    virtual void listSources() = 0;
    virtual void listAvailableApps() = 0;
    virtual void listInstalledApps() = 0;
    virtual void checkForUpdates() = 0;

    virtual QVector<SoftwareResource *> updates() const = 0;

    virtual bool addSource(const QString &source) = 0;
    virtual bool removeSource(SoftwareSource *source) = 0;

    virtual SoftwareResource *installAppFromPath(const QString &filePath) = 0;

Q_SIGNALS:
    void updatesAvailable(uint count);
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERBACKEND_H
