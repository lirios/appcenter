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

#include "screenshot.h"

#include "utils.h"

#include <QDebug>

using namespace Appstream;

Screenshot::Screenshot(QDomElement element)
{
    QStringList images = stringsByTagName(element, "image");
    if (!images.isEmpty())
        m_url = images.first();
    else
        m_url = element.text();

    QString type = element.attribute("type");

    if (type == "default")
        m_type = Screenshot::Default;
    else if (type == "normal")
        m_type = Screenshot::Normal;
    else
        m_type = Screenshot::Unknown;
}

bool Screenshot::operator==(const Screenshot &other) const
{
    return m_type == other.m_type && m_url == other.m_url;
}
