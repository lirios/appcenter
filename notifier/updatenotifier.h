/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef BACKEND_H
#define BACKEND_H

#include <QCoreApplication>
#include <QObject>

#include <Vibe/Core/Notification>

#include <Software/SoftwareManager>

class UpdateNotifier : public QObject
{
    Q_OBJECT

public:
    UpdateNotifier(QObject *parent = nullptr) : QObject(parent)
    {
        m_softwareManager = new SoftwareManager(this);
        connect(m_softwareManager, &SoftwareManager::updatesDownloaded, this,
                &UpdateNotifier::updatesDownloaded);
    }

public slots:
    void checkForUpdates()
    {
        qDebug() << "Checking for updates";
        m_softwareManager->downloadUpdates();
    }

private slots:
    void updatesDownloaded()
    {
        bool hasUpdates = m_softwareManager->hasUpdates();

        qDebug() << "Has updates" << hasUpdates;

        if (hasUpdates) {
            Vibe::Notification *notification =
                    new Vibe::Notification(this);
            notification->setApplicationName(QLatin1String("App Center"));
            notification->setApplicationIcon(QLatin1String("software-store"));
            notification->setSummary(tr("Updates available"));
            notification->setBody(m_softwareManager->updatesSummary());
            notification->setActions(QStringList(tr("Install updates")));
            // connect(notification, SIGNAL(activated(unsigned int )), contact ,
            // SLOT(slotOpenChat()) );
            notification->send();
        }
    }

private:
    SoftwareManager *m_softwareManager = nullptr;
};

#endif // BACKEND_H
