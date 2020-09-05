// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_ICON_H
#define LIRI_APPSTREAM_ICON_H

#include <QSharedDataPointer>

namespace Liri {

namespace AppStream {

class IconData;

class Icon
{
public:
    Icon();

private:
    QSharedDataPointer<IconData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_ICON_H
