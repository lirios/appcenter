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

#include "utils.h"

#include <QFile>

QStringList stringsByTagName(QDomElement element, QString tagName)
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

bool loadDocumentFromFile(QDomDocument *document, QString filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!document->setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool hasSuffix(QString filename, QStringList suffices)
{
    foreach (QString suffix, suffices) {
        if (filename.endsWith(suffix))
            return true;
    }

    return false;
}
