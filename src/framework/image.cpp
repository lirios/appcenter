// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
