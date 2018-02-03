/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include "image.h"
#include "image_p.h"

namespace Liri {

namespace AppCenter {

ImagePrivate::ImagePrivate()
{
}

Image::Image(const QUrl &url, const QSize &size)
    : d_ptr(new ImagePrivate())
{
    d_func()->url = url;
    d_func()->size = size;
}

Image::Image()
    : d_ptr(new ImagePrivate())
{
}

Image::Image(const Image &other)
    : d_ptr(new ImagePrivate())
{
    d_func()->url = other.url();
    d_func()->size = other.size();
}

Image::~Image()
{
    delete d_ptr;
}

bool Image::isValid() const
{
    Q_D(const Image);
    return d->url.isValid() && d->size.isValid();
}

QUrl Image::url() const
{
    Q_D(const Image);
    return d->url;
}

QSize Image::size() const
{
    Q_D(const Image);
    return d->size;
}

Image &Image::operator=(const Image &other)
{
    Q_D(Image);
    d->url = other.url();
    d->size = other.size();
    return *this;
}

} // namespace AppCenter

} // namespace Liri
