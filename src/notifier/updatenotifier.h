/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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
#include <QLoggingCategory>

#include <LiriAppCenter/SoftwareManager>
#include <LiriNotifications/Notification>

Q_DECLARE_LOGGING_CATEGORY(lcUpdateNotifier)

using namespace Liri;

class UpdateNotifier : public QObject
{
    Q_OBJECT
public:
    explicit UpdateNotifier(QObject *parent = nullptr);

    void initialize();

private Q_SLOTS:
    void updatesAvailable(uint count);
    void appUpdated();

private:
    AppCenter::SoftwareManager *m_softwareManager = nullptr;

    QString updatesSummary();
};

#endif // BACKEND_H
