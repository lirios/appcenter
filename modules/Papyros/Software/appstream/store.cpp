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

#include "store.h"

#include <QDir>
#include <QDebug>

using namespace Appstream;

bool Store::load(QString path)
{
    QDir dir(path);

    if (!dir.exists())
        return false;

    Q_FOREACH(QString filename, dir.entryList()) {
        Component component;

        if (!component.loadFromFile(path + "/" + filename))
            continue;

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

    return true;
}

Component Store::componentById(QString id) const
{
    Q_FOREACH(Component component, m_components) {
        if (component.m_id == id)
            return component;
    }

    return Component();
}
