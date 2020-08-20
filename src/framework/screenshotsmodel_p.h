// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSCREENSHOTSMODEL_P_H
#define LIRIAPPCENTERSCREENSHOTSMODEL_P_H

#include <LiriAppCenter/SoftwareResource>

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

class ScreenshotsModelPrivate
{
    Q_DECLARE_PUBLIC(ScreenshotsModel)
public:
    ScreenshotsModelPrivate(ScreenshotsModel *self);

    SoftwareResource *app = nullptr;
    QVector<Image> thumbnails;
    QVector<Image> screenshots;

protected:
    ScreenshotsModel *q_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSCREENSHOTSMODEL_P_H
