// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
