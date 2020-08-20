// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_CACHEDNETWORKACCESSMANAGER_H
#define LIRI_APPCENTER_CACHEDNETWORKACCESSMANAGER_H

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

#include <QNetworkAccessManager>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT CachedNetworkAccessManager : public QNetworkAccessManager
{
public:
    explicit CachedNetworkAccessManager(const QString &path, QObject *parent = nullptr);

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request,
                                 QIODevice *outgoingData = nullptr) override;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_CACHEDNETWORKACCESSMANAGER_H
