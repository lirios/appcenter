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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QString>
#include <QIcon>

#include <LiriModels/QQuickList>

#include <LiriSoftware/lirisoftwareglobal.h>
#include <LiriSoftware/SoftwareBackend>
#include <LiriSoftware/Screenshot>

namespace Appstream {
class Component;
}

class AppStreamScreenshot;

class LIRISOFTWARE_EXPORT Application : public QObject
{
    Q_OBJECT

    Q_PROPERTY(State state MEMBER m_state CONSTANT)
    Q_PROPERTY(Type type MEMBER m_type CONSTANT)
    Q_PROPERTY(QString id MEMBER m_id CONSTANT)
    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString summary MEMBER m_summary CONSTANT)
    Q_PROPERTY(QIcon icon READ icon CONSTANT)
    Q_PROPERTY(QString latestVersion READ latestVersion CONSTANT)
    Q_PROPERTY(QString installedVersion READ installedVersion CONSTANT)
    Q_PROPERTY(QObjectListModel *screenshots READ screenshots CONSTANT)
    Q_PROPERTY(bool updatesAvailable READ updatesAvailable CONSTANT)

public:
    enum State
    {
        Installed,
        Available
    };
    Q_ENUM(State)

    enum Type
    {
        App,
        Runtime
    };
    Q_ENUM(Type)

    Application(SoftwareBackend *backend) : QObject(backend), m_backend(backend) {}

    void refineFromAppstream(Appstream::Component component);

    virtual QString latestVersion() const = 0;
    virtual QString installedVersion() const = 0;
    virtual bool updatesAvailable() const = 0;

    QObjectListModel *screenshots() { return m_screenshots.getModel(); }

    QIcon icon()
    {
        if (m_icon.isNull())
            return QIcon::fromTheme("application-x-executable");
        else
            return m_icon;
    }

    State m_state;
    Type m_type;
    QString m_id;
    QString m_name;
    QString m_summary;
    QIcon m_icon;
    QQuickList<Screenshot> m_screenshots;

public slots:
    bool launch() const;

private:
    SoftwareBackend *m_backend;
};

#endif // APPLICATION_H
