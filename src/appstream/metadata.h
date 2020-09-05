// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_METADATA_H
#define LIRI_APPSTREAM_METADATA_H

#include <QSharedDataPointer>
#include <QUrl>

namespace Liri {

namespace AppStream {

class MetadataData;

class Metadata
{
public:
    enum Format {
        UnknownFormat,
        XmlFormat,
        YamlFormat,
        DesktopEntryFormat
    };

    enum Error {
        NoError,
        FileOpenError,
        ParseError,
    };

    Metadata();
    Metadata(const Metadata &other);

    QString fileName() const;

    QString version() const;
    QString origin() const;
    QUrl mediaBaseUrl() const;
    QString architecture() const;
    int priority() const;

    Error error() const;
    QString errorString() const;

    bool parseCollection(const QString &fileName, Format format = UnknownFormat);
    bool parseMetaInfo(const QString &fileName, Format format = UnknownFormat);

private:
    QSharedDataPointer<MetadataData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_METADATA_H
