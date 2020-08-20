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
