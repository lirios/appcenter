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

#ifndef APPSTREAM_UTILS_H
#define APPSTREAM_UTILS_H

#include <QObject>

#include <QString>
#include <QDomElement>

#include "autocast.h"

#define G_FOREACH(item, array)                                                                     \
    for (uint keep = 1, index = 0; keep && index < (array)->len; keep = !keep, index++)            \
        for (item = auto_cast(g_ptr_array_index(array, index)); keep; keep = !keep)

QStringList stringsByTagName(QDomElement element, QString tagName);

#endif // APPSTREAM_UTILS_H
