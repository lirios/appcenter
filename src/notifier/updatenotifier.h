// SPDX-FileCopyrightText: 2016 Michael Spencer <sonrisesoftware@gmail.com>
// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QLoggingCategory>

#include <LiriAppCenter/SoftwareManager>
#include <LiriNotifications/Notification>

Q_DECLARE_LOGGING_CATEGORY(lcUpdateNotifier)

using namespace Liri;

class UpdateNotifier : public QObject
{
    Q_OBJECT
public:
    explicit UpdateNotifier(QObject *parent = nullptr);

    void initialize();

private Q_SLOTS:
    void updatesAvailable(uint count);
    void appUpdated();

private:
    AppCenter::SoftwareManager *m_softwareManager = nullptr;

    QString updatesSummary();
};

#endif // BACKEND_H
