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

#include "store.h"

#include <QDir>
#include <QDebug>

#include <KArchive/KCompressionDevice>

#include "utils.h"

using namespace Appstream;

SourceKind Appstream::kindFromFilename(QString filename)
{
    if (hasSuffix(filename, {"xml.gz", "yml", "yml.gz"}))
        return Appstream::Appstream;
    else if (hasSuffix(filename, {"appdata.xml", "appdata.xml.in", "xml"}))
        return Appstream::Appdata;
    else if (hasSuffix(filename, {"metainfo.xml", "metainfo.xml.in"}))
        return Appstream::Metainfo;
    else if (hasSuffix(filename, {"desktop", "desktop.in"}))
        return Appstream::Desktop;
    else
        return Appstream::Unknown;
}

bool Store::load(QString path)
{
    QDir dir(path);

    if (!dir.exists())
        return false;

    Q_FOREACH (QString filename, dir.entryList()) {
        switch (kindFromFilename(filename)) {
        case Appstream::Appstream:
            loadFromAppstreamFile(path + "/" + filename, path + "/icons");
            break;
        default:
            Component component;

            if (!component.loadFromFile(path + "/" + filename))
                continue;

            addComponent(component);

            break;
        }
    }

    return true;
}

bool Store::loadFromAppstreamFile(QString filename, QString iconPath)
{
    qDebug() << "Loading from appstream file" << filename;
    QDomDocument doc("appstream");

    KCompressionDevice file(filename, KCompressionDevice::GZip);

    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file)) {
        file.close();
        qWarning() << "Unable to read from file:" << filename;
        return false;
    }
    file.close();

    QDomElement root = doc.firstChildElement("components");
    if (root.isNull())
        return false;

    FOREACH_ELEMENT (QDomElement element, root.elementsByTagName("component")) {
        Component component;

        if (!component.loadFromAppdata(element, iconPath))
            continue;

        addComponent(component);
    }

    return true;
}

void Store::addComponent(Component component)
{
    bool foundExisting = false;

    for (int i = 0; i < m_components.count(); i++) {
        Component &existingComponent = m_components[i];

        if (existingComponent.m_id == component.m_id) {
            existingComponent.merge(component);
            foundExisting = true;
        }
    }

    if (!foundExisting)
        m_components << component;
}

Component Store::componentById(QString id) const
{
    Q_FOREACH (Component component, m_components) {
        if (component.m_id == id)
            return component;
    }

    return Component();
}
