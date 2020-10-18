// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef INTERNETCHECK_H
#define INTERNETCHECK_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class InternetCheck : public QObject
{
    Q_OBJECT
public:
    explicit InternetCheck(QObject *parent = nullptr);

    bool hasInternet();

private:
    QNetworkAccessManager *m_manager = nullptr;

    QNetworkReply *asyncGet(const QUrl &url);
    QNetworkReply *syncGet(const QUrl &url);
};

#endif // INTERNETCHECK_H
