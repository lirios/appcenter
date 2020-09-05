// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QFile>
#include <QObject>
#include <QMimeDatabase>
#include <QMimeType>
#include <QXmlStreamReader>
#include <QSharedData>

#include "component.h"
#include "metadata.h"

namespace Liri {

namespace AppStream {

/*
 * MetadataData
 */

class MetadataData : public QSharedData
{
public:
    MetadataData()
    {
    }

    Metadata::Format detectFormat(const QString &fileName)
    {
        QMimeDatabase mimeDb;
        QMimeType mimeType = mimeDb.mimeTypeForFile(fileName);

        if (mimeType.inherits(QStringLiteral("application/x-yaml")))
            return Metadata::YamlFormat;
        else if (fileName.endsWith(QLatin1String(".desktop")))
            return Metadata::DesktopEntryFormat;
        else
            return Metadata::XmlFormat;
    }

    bool parseXmlCollection(QFile *file)
    {
        xml.setDevice(file);

        if (xml.readNextStartElement()) {
            if (xml.name() == QLatin1String("components")) {
                version = xml.attributes().value(QLatin1String("version")).toString();
                if (version.isEmpty()) {
                    xml.raiseError(QObject::tr("AppStream collection has missing version attribute"));
                    return false;
                }

                origin = xml.attributes().value(QLatin1String("origin")).toString();
                mediaBaseUrl = QUrl::fromUserInput(xml.attributes().value(QLatin1String("media_baseurl")).toString());
                architecture = xml.attributes().value(QLatin1String("architecture")).toString();
                priority = xml.attributes().value(QLatin1String("priority")).toInt();

                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("component")) {
                        if (!readComponent())
                            return false;
                    } else if (xml.name() == QLatin1String("id")) {
                        lastComponent.setId(xml.readElementText());
                    } else if (xml.name() == QLatin1String("pkgname")) {
                        lastComponent.addPackageName(xml.readElementText());
                    } else if (xml.name() == QLatin1String("source_pkgname")) {
                        lastComponent.setSourcePackageName(xml.readElementText());
                    } else if (xml.name() == QLatin1String("name")) {
                        lastComponent.setName(xml.readElementText());
                    } else if (xml.name() == QLatin1String("project_license")) {
                        lastComponent.setProjectLicense(xml.readElementText());
                    }
                }
            }
        }

        return !xml.hasError();
    }

    bool parseXmlMetaInfo(QFile *file)
    {
        xml.setDevice(file);

        while (xml.readNextStartElement()) {
            if (xml.name() == QLatin1String("component")) {
                if (!readComponent())
                    return false;
            }
        }

        return !xml.hasError();
    }

    bool readComponent()
    {
        auto type = xml.attributes().value(QLatin1String("type")).toString();
        auto merge = xml.attributes().value(QLatin1String("merge")).toString();

        lastComponent.setType(Component::stringToType(type));
        lastComponent.setPriority(xml.attributes().value(QLatin1String("priority")).toInt());
        lastComponent.setMerge(Component::stringToMerge(merge));

        return !xml.hasError();
    }

    QString fileName;
    Metadata::Error error = Metadata::NoError;
    QString errorString;
    QXmlStreamReader xml;
    QString version;
    QString origin;
    QUrl mediaBaseUrl;
    QString architecture;
    int priority = 0;
    QHash<QString, Component> components;
    Component lastComponent;

    QString lastComponentType;
    int lastComponentPriority = 0;
    QString lastComponentMerge;
};

/*
 * Metadata
 */

Metadata::Metadata()
    : d(new MetadataData())
{
}

Metadata::Metadata(const Metadata &other)
    : d(other.d)
{
}

QString Metadata::fileName() const
{
    return d->fileName;
}

QString Metadata::version() const
{
    return d->version;
}

QString Metadata::origin() const
{
    return d->origin;
}

QUrl Metadata::mediaBaseUrl() const
{
    return d->mediaBaseUrl;
}

QString Metadata::architecture() const
{
    return d->architecture;
}

int Metadata::priority() const
{
    return d->priority;
}

Metadata::Error Metadata::error() const
{
    return d->error;
}

QString Metadata::errorString() const
{
    return d->errorString;
}

bool Metadata::parseCollection(const QString &fileName, Metadata::Format format)
{
    Format actualFormat = format;

    // Detect the format
    if (format == UnknownFormat)
        actualFormat = d->detectFormat(fileName);

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        d->error = FileOpenError;
        d->errorString = QObject::tr("Failed to open collection: %1").arg(file.errorString());
        return false;
    }

    bool result = false;

    if (actualFormat == XmlFormat)
        result = d->parseXmlCollection(&file);

    file.close();

    return result;
}

bool Metadata::parseMetaInfo(const QString &fileName, Metadata::Format format)
{
    Format actualFormat = format;

    // Detect the format
    if (format == UnknownFormat)
        actualFormat = d->detectFormat(fileName);

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        d->error = FileOpenError;
        d->errorString = QObject::tr("Failed to open metainfo: %1").arg(file.errorString());
        return false;
    }

    bool result = false;

    if (actualFormat == XmlFormat)
        result = d->parseXmlMetaInfo(&file);

    file.close();

    return result;
}

} // namespace AppStream

} // namespace Liri
