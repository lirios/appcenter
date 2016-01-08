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

#ifndef XDG_APPLICATION_H
#define XDG_APPLICATION_H

#include "application.h"

#include <QObject>
#include <QString>

#include <AppstreamQt/component.h>

#include "base.h"

class XdgApplication: public Application
{
    Q_OBJECT

    Q_PROPERTY(QString branch MEMBER m_branch CONSTANT)
    Q_PROPERTY(QString origin MEMBER m_origin CONSTANT)
    Q_PROPERTY(QString arch MEMBER m_arch CONSTANT)

public:
    XdgApplication(XdgAppInstalledRef *app_ref, State state, QObject *parent = nullptr);

    QString iconName() const override
    {
        return m_type == Application::Runtime ? "application-x-executable"
                                              : m_component.icon();
    }

    QString installedVersion() const override
    {
        return m_branch;
    }

    QString latestVersion() const override
    {
        return "UNKNOWN";
    }

    Q_INVOKABLE QUrl iconUrl(const QSize& size) const
    {
        return m_component.iconUrl(size);
    }

    Appstream::Component m_component;
    QString m_branch;
    QString m_origin;
    QString m_arch;

public slots:
    void install();

private:
    static Appstream::Component findComponent(QStringList paths, QString id);
};

#endif // XDG_APPLICATION_H
