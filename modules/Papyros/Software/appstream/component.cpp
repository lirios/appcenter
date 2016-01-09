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

#include "component.h"

#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QDir>
#include <QDebug>
#include <QSettings>

#define MERGE_FIELD(other, fieldName) if (fieldName.isEmpty()) fieldName = other.fieldName;
#define MERGE_LIST_FIELD(other, fieldName) mergeLists(fieldName, other.fieldName);
#define MERGE_HASH_FIELD(other, fieldName) mergeHashes(fieldName, other.fieldName);

using namespace Appstream;

template<typename T>
void mergeLists(QList<T> &list1, const QList<T> &list2)
{
    Q_FOREACH(T item, list2) {
        if (!list1.contains(item))
            list1 << item;
    }
}

template<typename T, typename E>
void mergeHashes(QHash<T, QList<E>> &hash1, const QHash<T, QList<E>> &hash2)
{
    Q_FOREACH(T key, hash2.keys()) {
        if (hash1.contains(key))
            hash1[key] << hash2[key];
        else
            hash1[key] = hash2[key];
    }
}

template<typename T, typename E>
void mergeHashes(QHash<T, E> &hash1, const QHash<T, E> &hash2)
{
    Q_FOREACH(T key, hash2.keys()) {
        if (!hash1.contains(key))
            hash1[key] = hash2[key];
    }
}

QStringList stringsByTagName(QDomElement element, QString tagName) {
    QStringList strings;

    QDomNodeList nodes = element.elementsByTagName(tagName);
    for (int i = 0; i < nodes.count(); i++) {
        QDomElement subElement = nodes.at(i).toElement();
        if (!subElement.isNull())
            strings << subElement.text();
    }

    return strings;
}

bool hasSuffix(QString filename, QStringList suffices)
{
    Q_FOREACH(QString suffix, suffices) {
        if (filename.endsWith(suffix))
            return true;
    }

    return false;
}

Component::SourceKind kindFromFilename(QString filename)
{
    if (hasSuffix(filename, {"xml.gz", "yml", "yml.gz"}))
        return Component::Appstream;
    else if (hasSuffix(filename, {"appdata.xml", "appdata.xml.in", "xml"}))
        return Component::Appdata;
    else if (hasSuffix(filename, {"metainfo.xml", "metainfo.xml.in"}))
        return Component::Metainfo;
    else if (hasSuffix(filename, {"desktop", "desktop.in"}))
        return Component::Desktop;
    else
        return Component::Unknown;
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

QString defaultLocale() {
    return QLocale::system().name();
}

template<typename T>
T lookupLocale(QHash<QString, T> hash, QString locale) {
    QStringList locales = {QLocale::system().name(), QLocale::c().name()};
    if (!locale.isEmpty())
        locales.insert(0, locale);
    qDebug() << locales << hash.keys();
    Q_FOREACH(QString possibleLocale, locales) {
        if (hash.contains(possibleLocale))
            return hash[possibleLocale];
    }
    return T();
}

template<typename T>
void removeItems(QList<T> *list, QList<T> sublist)
{
    Q_FOREACH(T item, sublist) {
        list->removeAll(item);
    }
}

void Component::merge(const Component &other)
{
    MERGE_FIELD(other, m_id);
    MERGE_FIELD(other, m_kind);
    if (m_priority == -1) m_priority = other.m_priority;
    MERGE_LIST_FIELD(other, m_packageNames);
    MERGE_LIST_FIELD(other, m_categories);
    MERGE_LIST_FIELD(other, m_architectures);
    MERGE_LIST_FIELD(other, m_kudos);
    MERGE_LIST_FIELD(other, m_permissions);
    MERGE_LIST_FIELD(other, m_vetos);
    MERGE_LIST_FIELD(other, m_mimetypes);
    MERGE_FIELD(other, m_projectLicense);
    MERGE_FIELD(other, m_metadataLicense);
    MERGE_FIELD(other, m_sourcePackageName);
    MERGE_FIELD(other, m_updateContact);
    MERGE_FIELD(other, m_projectGroup);
    MERGE_LIST_FIELD(other, m_compulsoryForDesktops);
    MERGE_LIST_FIELD(other, m_extends);
    MERGE_LIST_FIELD(other, m_urls);
    MERGE_HASH_FIELD(other, m_names);
    MERGE_HASH_FIELD(other, m_comments);
    MERGE_HASH_FIELD(other, m_developerNames);
    MERGE_HASH_FIELD(other, m_keywords);
    MERGE_FIELD(other, m_iconName);
}

bool Component::loadFromFile(QString filename)
{
    switch (kindFromFilename(filename)) {
    case Component::Appdata:
    case Component::Metainfo:
        return loadFromAppdataFile(filename);
    case Component::Desktop:
        return loadFromDesktopFile(filename);
    default:
        return false;
    }
}

bool Component::loadFromAppdataFile(QString filename)
{
    qDebug() << "Loading from appdata file!";
    QDomDocument doc("appdata");

    if (!loadDocumentFromFile(&doc, filename))
        return false;

    QDomElement appNode = doc.firstChildElement("application");
    if (appNode.isNull())
        appNode = doc.firstChildElement("component");
    if (appNode.isNull())
        return false;

    for (QDomNode node = appNode.firstChild(); !node.isNull(); node = node.nextSibling()) {
        QDomElement element = node.toElement();

        if (element.isNull())
            continue;

        QString tagName = element.tagName();
        QString text = element.text();

        // qDebug() << tagName << text;

        if (tagName == "id") {
            m_id = text;
            QString kind = element.attribute("type");
            if (!kind.isEmpty())
                m_kind = kind;
        } else if (tagName == "priority") {
            m_priority = text.toInt();
        } else if (tagName == "pkgname") {
            m_packageNames << text;
        } else if (tagName == "bundle") {
            // TODO: Parse and add the bundle
        } else if (tagName == "name") {
            QString language = element.attribute("xml:lang");
            m_names[language] = text;
        } else if (tagName == "summary") {
            QString language = element.attribute("xml:lang");
            m_comments[language] = text;
        } else if (tagName == "developer_name") {
            QString language = element.attribute("xml:lang");
            m_developerNames[language] = text;
        } else if (tagName == "description") {
            qDebug() << "WARNING: description not parsed";
        } else if (tagName == "icon") {
            qDebug() << "WARNING: icon not parsed";
        } else if (tagName == "categories") {
            m_categories << stringsByTagName(element, "category");
        } else if (tagName == "architectures") {
            m_architectures << stringsByTagName(element, "architecture");
        } else if (tagName == "keywords") {
            QDomNodeList keywords = doc.elementsByTagName("keyword");
            for (int i = 0; i < keywords.count(); i++) {
                QDomElement keyword = keywords.at(i).toElement();
                if (!keyword.isNull()) {
                    QString language = element.attribute("xml:lang");
                    addKeyword(keyword.text(), language);
                }
            }
        } else if (tagName == "kudos") {
            m_kudos << stringsByTagName(element, "kudo");
        } else if (tagName == "permissions") {
            m_permissions << stringsByTagName(element, "permission");
        } else if (tagName == "vetos") {
            m_vetos << stringsByTagName(element, "veto");
        } else if (tagName == "mimetypes") {
            m_mimetypes << stringsByTagName(element, "mimetype");
        } else if (tagName == "project_license") {
            m_projectLicense = text;
        } else if (tagName == "metadata_license") {
            m_metadataLicense = text;
        } else if (tagName == "source_pkgname") {
            m_sourcePackageName = text;
        } else if (tagName == "updatecontact" || tagName == "update_contact") {
            m_updateContact = text;
        } else if (tagName == "url") {
            m_urls << Url(text, element.attribute("type"));
        } else if (tagName == "project_group") {
            m_projectGroup = text;
        } else if (tagName == "compulsory_for_desktop") {
            m_compulsoryForDesktops << text;
        } else if (tagName == "extends") {
            m_extends << text;
        } else if (tagName == "screenshots") {
            qDebug() << "WARNING: screenshots not parsed";
        } else if (tagName == "releases") {
            qDebug() << "WARNING: releases not parsed";
        } else if (tagName == "provides") {
            qDebug() << "WARNING: provides not parsed";
        } else if (tagName == "languages") {
            qDebug() << "WARNING: languages not parsed";
        } else if (tagName == "metadata") {
            qDebug() << "WARNING: metadata not parsed";
        } else {
            qFatal("Tag not supported: %s", qPrintable(tagName));
        }
    }

    return true;
}

bool Component::loadFromDesktopFile(QString filename)
{
    QFileInfo fileInfo(filename);

    m_id = fileInfo.fileName();

    QSettings settings(filename, QSettings::IniFormat);

    settings.beginGroup("Desktop Entry");

    Q_FOREACH(QString key, settings.allKeys()) {
        QString value = settings.value(key).toString();

        // qDebug() << key << value;

        if (key == "NoDisplay") {
            if (value == "True")
                m_vetos << "NoDisplay=true";
        } else if (key == "Type") {
            if (value != "Application")
                return false;
        } else if (key == "Icon") {
            QFileInfo info(value);

            if (info.isAbsolute())
                m_iconName = value;
            else if (hasSuffix(value, {"png", "xpm", "svg"}))
                m_iconName = info.completeBaseName();
            else
                m_iconName = value;
        } else if (key == "Categories") {
            QStringList categories = value.split(";");
            QStringList ignoredCategories = {"GTK", "Qt", "KDE", "GNOME"};
            removeItems(&categories, ignoredCategories);
            m_categories << categories;
        } else if (key == "Keywords") {
            qDebug() << "WARNING: Name skipped";
        } else if (key == "MimeType") {
            qDebug() << "WARNING: Name skipped";
        } else if (key == "OnlyShowIn") {
            QStringList desktops = value.split(";");
            if (desktops.count() == 1)
                m_projectGroup = desktops[0];
        } else if (key == "Name" || key == "_Name") {
            m_names["C"] = value;
        } else if (key.startsWith("Name")) {
            qDebug() << "WARNING: Name skipped";
        } else if (key == "Comment" || key == "_Comment") {
            m_comments["C"] = value;
        } else if (key.startsWith("Comment")) {
            qDebug() << "WARNING: Comment skipped";
        } else if (key == "X-AppInstall-Package") {
            m_packageNames << value;
        } else if (key == "X-Ubuntu-Software-Center-Name") {
            m_names["C"] = value;
        } else if (key.startsWith("X-Ubuntu-Software-Center-Name")) {
            qDebug() << "WARNING: X-Ubuntu-Software-Center-Name skipped";
        }
    }

    settings.endGroup();

    return true;
}

void Component::addKeyword(QString keyword, QString locale) {
    QStringList keywords = m_keywords.contains(locale) ? m_keywords[locale] : QStringList();

    keywords << keyword;

    m_keywords[locale] = keywords;
}

QString Component::name(QString locale) const {
    return lookupLocale(m_names, locale);
}

QString Component::comment(QString locale) const {
    return lookupLocale(m_comments, locale);
}

QString Component::developerName(QString locale) const {
    return lookupLocale(m_developerNames, locale);
}

QStringList Component::keywords(QString locale) const {
    return lookupLocale(m_keywords, locale);
}
