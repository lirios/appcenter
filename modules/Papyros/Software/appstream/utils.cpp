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

#include "utils.h"

QStringList Appstream::stringsByTagName(QDomElement element, QString tagName)
{
    QStringList strings;

    QDomNodeList nodes = element.elementsByTagName(tagName);
    for (int i = 0; i < nodes.count(); i++) {
        QDomElement subElement = nodes.at(i).toElement();
        if (!subElement.isNull())
            strings << subElement.text();
    }

    return strings;
}
