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
#include "xdg-app.h"

#include <QProcess>
#include <QStringList>

XdgApp::XdgApp(QObject *parent)
    : QObject(parent)
{
    // Nothing needed here yet.
}

void XdgApp::addRemote(QString remoteName, QString url) const
{
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList() << "add-remote" << remoteName << url);
        return;
        // TODO: Error checking
}

void XdgApp::installApp(QString remoteName, QString appName) const
{
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList() << "install-app" << remoteName << appName);
        return;
}

void XdgApp::updateApp(QString appName) const
{
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList() << "update-app" << appName);
        return;
}

void XdgApp::installRuntime(QString remoteName, QString runtimeName) const
{
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList() << "install-runtime" <<remoteName << runtimeName);
        return;
}

QStringList XdgApp::listApps() const
{
        QProcess *process = new QProcess();
        process->start("xdg-app", QStringList() << "list-apps");
        process->waitForFinished();
        QString output(process->readAllStandardOutput());
        QStringList apps = output.split("\n");
        return apps;
}
