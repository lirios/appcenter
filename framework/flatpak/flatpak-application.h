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

#ifndef FLATPAK_APPLICATION_H
#define FLATPAK_APPLICATION_H

#include "application.h"

#include <QObject>
#include <QString>

#include "appstream/component.h"
#include "base.h"

class FlatpakApplication : public Application
{
    Q_OBJECT

    Q_PROPERTY(QString branch MEMBER m_branch CONSTANT)
    Q_PROPERTY(QString origin MEMBER m_origin CONSTANT)
    Q_PROPERTY(QString arch MEMBER m_arch CONSTANT)

public:
    FlatpakApplication(FlatpakInstalledRef *app_ref, SoftwareBackend *backend);
    FlatpakApplication(Appstream::Component component, QString origin, SoftwareBackend *backend);

    QString installedVersion() const override { return m_branch; }

    QString latestVersion() const override { return "UNKNOWN"; }

    bool updatesAvailable() const override { return m_currentCommit != m_latestCommit; }

    QString m_branch;
    QString m_origin;
    QString m_arch;
    QString m_currentCommit;
    QString m_latestCommit;

public slots:
    void install();
};

#endif // FLATPAK_APPLICATION_H
