// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_SUGGESTED_H
#define LIRI_APPSTREAM_SUGGESTED_H

#include <QSharedDataPointer>

namespace Liri {

namespace AppStream {

class SuggestedData;

class Suggested
{
public:
    Suggested();

private:
    QSharedDataPointer<SuggestedData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_SUGGESTED_H
