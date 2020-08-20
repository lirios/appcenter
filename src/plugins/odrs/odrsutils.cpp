// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QCryptographicHash>
#include <QFile>

#include <Qt5AccountsService/UserAccount>

#include "odrsutils.h"

namespace Liri {

namespace AppCenter {

QString getUserHash()
{
    // Read machine-id
    QString machineId;
    QFile machineIdFile(QStringLiteral("/etc/machine-id"));
    if (machineIdFile.open(QFile::ReadOnly)) {
        machineId = QString::fromUtf8(machineIdFile.readAll());
        machineIdFile.close();
    }

    if (machineId.isEmpty())
        return QString();

    auto *account = new QtAccountsService::UserAccount();
    QString salted = QStringLiteral("gnome-software[%1:%2]").arg(account->userName(), machineId);
    return QString::fromUtf8(QCryptographicHash::hash(salted.toUtf8(), QCryptographicHash::Sha1).toHex());
}

} // namespace AppCenter

} // namespace Liri
