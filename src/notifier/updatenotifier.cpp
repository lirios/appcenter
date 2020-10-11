// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTimer>

#include <LiriAppCenter/SoftwareResource>

#include "updatenotifier.h"

Q_LOGGING_CATEGORY(lcUpdateNotifier, "liri.appcenter", QtInfoMsg)

UpdateNotifier::UpdateNotifier(QObject *parent)
    : QObject(parent)
    , m_softwareManager(new AppCenter::SoftwareManager(this))
{
    connect(m_softwareManager, &AppCenter::SoftwareManager::updatesAvailable,
            this, &UpdateNotifier::updatesAvailable);
}

void UpdateNotifier::initialize()
{
    // Initialize
    m_softwareManager->initialize();

    // Check for updates every hour
    // TODO: The interval should be configurable
    QTimer *timer = new QTimer(this);
    timer->setInterval(1 * 60 * 60 * 1000);
    connect(timer, &QTimer::timeout,
            m_softwareManager,
            &AppCenter::SoftwareManager::checkForUpdates);
}

void UpdateNotifier::updatesAvailable(uint count)
{
    if (count == 0)
        return;

    const AppCenter::SoftwareResources updates = m_softwareManager->updates();

    for (const auto &app : updates) {
        connect(app, &AppCenter::SoftwareResource::updated,
                this, &UpdateNotifier::appUpdated);
    }

    Notification *notification = new Notification(this);
    connect(notification, &Notification::actionInvoked, this,
            [updates, notification](const QString &actionId) {
        if (actionId == QLatin1String("default") || actionId == QLatin1String("2")) {
            for (const auto &app : updates) {
                if (!app->update())
                    qCWarning(lcUpdateNotifier, "Failed to update \"%s\"",
                              qPrintable(app->name()));
            }
        }
        notification->deleteLater();
    });
    notification->setApplicationName(QLatin1String("App Center"));
    notification->setApplicationIcon(QLatin1String("software-store"));
    notification->setSummary(tr("Updates available"));
    notification->setBody(updatesSummary());
    notification->setActions(QStringList{tr("Not now"), tr("Install updates")});
    notification->setDefaultAction(tr("Install updates"));
    notification->send();
}

void UpdateNotifier::appUpdated()
{
    auto *app = qobject_cast<AppCenter::SoftwareResource *>(sender());
    disconnect(app, &AppCenter::SoftwareResource::updated,
               this, &UpdateNotifier::appUpdated);
    qCInfo(lcUpdateNotifier, "\"%s\" updated successfully!",
           qPrintable(app->name()));
}

QString UpdateNotifier::updatesSummary()
{
    AppCenter::SoftwareResources updates = m_softwareManager->updates();
    int updatesCount = updates.size();

    if (updatesCount == 0) {
        return tr("No updates are available");
    } else if (updatesCount == 1) {
        return tr("%1 is ready to update").arg(updates.at(0)->name());
    } else if (updatesCount == 2) {
        return tr("%1 and %2 are ready to update").arg(updates.at(0)->name(), updates.at(1)->name());
    } else if (updatesCount == 3) {
        return tr("%1, %2, and one other app are ready to update").arg(updates.at(0)->name(), updates.at(1)->name());
    } else {
        int otherCount = updatesCount - 2;
        return tr("%1, %2, and %3 other apps are ready to update").arg(updates.at(0)->name(), updates.at(1)->name()).arg(otherCount);
    }
}
