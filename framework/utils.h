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

#define FOREACH_ELEMENT(item, list)                                                                \
    for (uint keep = 1, index = 0; keep && index < static_cast<uint>((list).count());              \
         keep = !keep, index++)                                                                    \
        for (item = (list).at(index).toElement(); keep; keep = !keep)

QStringList stringsByTagName(QDomElement element, QString tagName);

bool loadDocumentFromFile(QDomDocument *document, QString filename);

bool hasSuffix(QString filename, QStringList suffices);

template <typename T>
void mergeLists(QList<T> &list1, const QList<T> &list2)
{
    foreach (T item, list2) {
        if (!list1.contains(item))
            list1 << item;
    }
}

template <typename T, typename E>
void mergeHashes(QHash<T, QList<E>> &hash1, const QHash<T, QList<E>> &hash2)
{
    foreach (T key, hash2.keys()) {
        if (hash1.contains(key))
            hash1[key] << hash2[key];
        else
            hash1[key] = hash2[key];
    }
}

template <typename T, typename E>
void mergeHashes(QHash<T, E> &hash1, const QHash<T, E> &hash2)
{
    foreach (T key, hash2.keys()) {
        if (!hash1.contains(key))
            hash1[key] = hash2[key];
    }
}

#endif // APPSTREAM_UTILS_H
