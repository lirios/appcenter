// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "screenshot.h"

namespace Liri {

namespace AppStream {

/*
 * ScreenshotData
 */

class ScreenshotData : public QSharedData
{
public:
    ScreenshotData()
    {
    }
};

/*
 * Screenshot
 */

Screenshot::Screenshot()
    : d(new ScreenshotData())
{
}

} // namespace AppStream

} // namespace Liri
