// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QtMath>
#include <QNetworkDiskCache>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QStorageInfo>

#include "cachednetworkaccessmanager_p.h"

namespace Liri {

namespace AppCenter {

CachedNetworkAccessManager::CachedNetworkAccessManager(const QString &path, QObject *parent)
    : QNetworkAccessManager(parent)
{
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
            QLatin1Char('/') + path;

    QStorageInfo storageInfo(cacheDir);
    QNetworkDiskCache *cache = new QNetworkDiskCache(this);
    cache->setCacheDirectory(cacheDir);
    cache->setMaximumCacheSize(qFloor(storageInfo.bytesAvailable() * 0.5f));
    setCache(cache);
}

QNetworkReply *CachedNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                         const QNetworkRequest &request,
                                                         QIODevice *outgoingData)
{
    QNetworkRequest cachedRequest(request);
    cachedRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    return QNetworkAccessManager::createRequest(op, cachedRequest, outgoingData);
}

} // namespace AppCenter

} // namespace Liri
