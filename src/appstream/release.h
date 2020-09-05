// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_RELEASE_H
#define LIRI_APPSTREAM_RELEASE_H

#include <QSharedDataPointer>

namespace Liri {

namespace AppStream {

class ReleaseData;

class Release
{
public:
    Release();

private:
    QSharedDataPointer<ReleaseData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_RELEASE_H
