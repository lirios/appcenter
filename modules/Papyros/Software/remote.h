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

#ifndef REMOTE_H
#define REMOTE_H

#include <QObject>
#include <QString>

#include "base.h"

class Remote: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString title MEMBER m_title CONSTANT)
    Q_PROPERTY(QString url MEMBER m_url CONSTANT)

public:
    Remote(XdgAppRemote *remote, QObject *parent = nullptr)
        : QObject(parent)
    {
        m_name = xdg_app_remote_get_name(remote);
        m_title = xdg_app_remote_get_title(remote);
        m_url = xdg_app_remote_get_url(remote);
    }

    QString m_name;
    QString m_title;
    QString m_url;
};

#endif // REMOTE_H
