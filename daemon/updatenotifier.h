/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
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

#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include <QCoreApplication>
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
    void updatesDownloaded(bool hasUpdates)
    {
        qDebug() << "Has updates" << hasUpdates;
        QCoreApplication::instance()->quit();
    }

private:
    SoftwareManager *m_softwareManager = nullptr;
};

#endif // BACKEND_H
