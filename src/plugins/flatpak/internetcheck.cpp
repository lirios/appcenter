// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QEventLoop>
#include <QTimer>

#include "flatpakbackend.h"
#include "internetcheck.h"

InternetCheck::InternetCheck(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
{
}

bool InternetCheck::hasInternet()
{
    if (syncGet(QUrl(QStringLiteral("http://google.com"))))
        return true;
    return false;
}

QNetworkReply *InternetCheck::asyncGet(const QUrl &url)
{
    QNetworkRequest request(url);

    auto *reply = m_manager->get(request);
    if (reply->error()) {
        reply->deleteLater();
        return reply;
    }

    auto *timer = new QTimer(reply);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    timer->start(30000);

    return reply;
}

QNetworkReply *InternetCheck::syncGet(const QUrl &url)
{
    auto *reply = asyncGet(url);
    if (!reply)
        return nullptr;

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    reply->deleteLater();
    if (reply->isRunning()) {
        qCWarning(lcFlatpakBackend, "Timeout on request to %s",
                  qPrintable(url.toString()));
        return nullptr;
    } else if (reply->error() != QNetworkReply::NoError) {
        qCWarning(lcFlatpakBackend, "Error on request to %s: %s",
                  qPrintable(url.toString()),
                  qPrintable(reply->errorString()));
        return nullptr;
    }

    return reply;
}
