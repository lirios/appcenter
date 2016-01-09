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

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>

#include <QString>
#include <QHash>
#include <QDomElement>

namespace Appstream
{

class Screenshot: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url MEMBER m_url CONSTANT)
    
public:
    enum Type {
        Normal, Default, Unknown
    };

    Screenshot(QDomElement element, QObject *parent = nullptr);

    // bool operator==(const Screenshot &other) const;

    QString m_url;
    Type m_type;
    int m_priority;
    QHash<QString,QString> m_captions;
};

}

#endif // SCREENSHOT_H
