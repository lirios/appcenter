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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

#include <QDomDocument>
#include <QString>
#include <QHash>

#include "url.h"

namespace Appstream
{

class Component
{

public:
    enum SourceKind {
        Appstream,
        Appdata,
        Metainfo,
        Desktop,
        Unknown
    };

    void merge(const Component &other);

    bool loadFromFile(QString filename);

    bool isNull() const { return m_id.isEmpty(); }
    QString name(QString locale = "") const;
    QString comment(QString locale = "") const;
    QString developerName(QString locale = "") const;
    QStringList keywords(QString locale = "") const;

    QString m_id;
    QString m_kind;
    int m_priority = -1;
    QStringList m_packageNames;
    QStringList m_categories;
    QStringList m_architectures;
    QStringList m_kudos;
    QStringList m_permissions;
    QStringList m_vetos;
    QStringList m_mimetypes;
    QString m_projectLicense;
    QString m_metadataLicense;
    QString m_sourcePackageName;
    QString m_updateContact;
    QString m_projectGroup;
    QStringList m_compulsoryForDesktops;
    QStringList m_extends;
    QList<Url> m_urls;
    QString m_iconName;

private:
    bool loadFromAppdataFile(QString filename);
    bool loadFromDesktopFile(QString filename);

    void addKeyword(QString keyword, QString locale);

    QHash<QString,QString> m_names;
    QHash<QString,QString> m_comments;
    QHash<QString,QString> m_developerNames;
    QHash<QString,QStringList> m_keywords;
};

}

#endif // COMPONENT_H
