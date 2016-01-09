/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 * Copyright (C) 2013-2015 Richard Hughes <richard@hughsie.com>
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

#ifndef URL_H
#define URL_H

#include <QObject>

#include <QString>

namespace Appstream
{

class Url
{
public:
    enum Type {
        Homepage, BugTracker, FAQ, Donation, Help, Missing, Unknown
    };

    Url(QString url, Type type);
    Url(QString url, QString type);

    bool operator==(const Url &other) const;

    QString m_url;
    Type m_type;
};

}

#endif // URL_H
