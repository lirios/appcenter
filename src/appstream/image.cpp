// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "image.h"

namespace Liri {

namespace AppStream {

/*
 * ImageData
 */

class ImageData : public QSharedData
{
public:
    ImageData()
    {
    }
};

/*
 * Image
 */

Image::Image()
    : d(new ImageData())
{
}

} // namespace AppStream

} // namespace Liri
