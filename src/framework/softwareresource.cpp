/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "softwareresource.h"
#include "softwareresource_p.h"

namespace Liri {

namespace AppCenter {

SoftwareResourcePrivate::SoftwareResourcePrivate()
{
}

SoftwareResource::SoftwareResource(QObject *parent)
    : QObject(parent)
    , d_ptr(new SoftwareResourcePrivate())
{
}

SoftwareResource::~SoftwareResource()
{
    delete d_ptr;
}

QString SoftwareResource::iconName() const
{
    if (iconNames().size() > 0) {
        for (const auto &name : iconNames()) {
            if (!name.isEmpty())
                return name;
        }
    }
    return QLatin1String("package-x-generic");
}

QUrl SoftwareResource::iconUrl() const
{
    if (iconUrls().size() > 0) {
        for (const auto &url : iconUrls()) {
            if (url.isValid())
                return url;
        }
    }
    return QUrl();
}

QString SoftwareResource::category() const
{
    return categories().join(QString::fromUtf8(" → "));
}

QString SoftwareResource::version() const
{
    if (state() == BrokenState)
        return QString();
    else if (state() == InstalledState)
        return installedVersion();
    return availableVersion();
}

bool SoftwareResource::launch() const
{
    return false;
}

bool SoftwareResource::install()
{
    return false;
}

bool SoftwareResource::uninstall()
{
    return false;
}

bool SoftwareResource::update()
{
    return false;
}

void SoftwareResource::addMetadata(const QString &key, const QVariant &value)
{
    Q_D(SoftwareResource);
    d->metadata.insert(key, value);
}

QVariant SoftwareResource::getMetadata(const QString &key)
{
    Q_D(SoftwareResource);
    return d->metadata.value(key);
}

} // namespace AppCenter

} // namespace Liri
