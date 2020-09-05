// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "category.h"

namespace Liri {

namespace AppStream {

/*
 * CategoryData
 */

class CategoryData : public QSharedData
{
public:
    CategoryData()
    {
    }
};

/*
 * Category
 */

Category::Category()
    : d(new CategoryData())
{
}

} // namespace AppStream

} // namespace Liri
