// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERTRANSACTION_P_H
#define LIRIAPPCENTERTRANSACTION_P_H

#include <LiriAppCenter/Transaction>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Liri {

namespace AppCenter {

class TransactionPrivate
{
public:
    TransactionPrivate();

    Transaction::Type type;
    QString name;
    QString description;
    SoftwareResource *resource = nullptr;
    Transaction::Status status = Transaction::Starting;
    QString errorMessage;
    bool cancellable = false;
    bool shouldCancel = true;
    int progress = 0;
    bool visible = true;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERTRANSACTION_P_H
