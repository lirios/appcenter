// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "softwaresource.h"
#include "softwaresource_p.h"

namespace Liri {

namespace AppCenter {

SoftwareSource::SoftwareSource(Backend *backend, QObject *parent)
    : QObject(parent)
    , d_ptr(new SoftwareSourcePrivate())
{
    d_ptr->backend = backend;
}

SoftwareSource::~SoftwareSource()
{
    delete d_ptr;
}

Backend *SoftwareSource::backend() const
{
    Q_D(const SoftwareSource);
    return d->backend;
}

QString SoftwareSource::hostname() const
{
    return url().host();
}

} // namespace AppCenter

} // namespace Liri
