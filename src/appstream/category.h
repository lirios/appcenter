// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_CATEGORY_H
#define LIRI_APPSTREAM_CATEGORY_H

#include <QSharedDataPointer>

namespace Liri {

namespace AppStream {

class CategoryData;

class Category
{
public:
    Category();

private:
    QSharedDataPointer<CategoryData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_CATEGORY_H
