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

#ifndef LIRIAPPCENTERIMAGE_H
#define LIRIAPPCENTERIMAGE_H

#include <QUrl>
#include <QSize>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class ImagePrivate;

class LIRIAPPCENTER_EXPORT Image
{
    Q_GADGET
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)
    Q_PROPERTY(QSize size READ size CONSTANT)
    Q_DECLARE_PRIVATE(Image)
public:
    explicit Image(const QUrl &url, const QSize &size);
    Image();
    Image(const Image &other);
    ~Image();

    bool isValid() const;

    QUrl url() const;
    QSize size() const;

    Image &operator=(const Image &other);

private:
    ImagePrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERIMAGE_H
