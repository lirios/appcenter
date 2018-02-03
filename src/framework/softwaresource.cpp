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

SoftwareSourcePrivate::SoftwareSourcePrivate()
{
}

SoftwareSource::SoftwareSource(QObject *parent)
    : QObject(parent)
    , d_ptr(new SoftwareSourcePrivate())
{
}

SoftwareSource::~SoftwareSource()
{
    delete d_ptr;
}

QObject *SoftwareSource::backend() const
{
    Q_D(const SoftwareSource);
    return d->backend;
}

void SoftwareSource::setBackend(QObject *backend)
{
    Q_D(SoftwareSource);

    if (d->backend == backend)
        return;

    d->backend = backend;
    Q_EMIT backendChanged();
}

QString SoftwareSource::name() const
{
    Q_D(const SoftwareSource);
    return d->name;
}

void SoftwareSource::setName(const QString &name)
{
    Q_D(SoftwareSource);

    if (d->name == name)
        return;

    d->name = name;
    Q_EMIT nameChanged();
}

QString SoftwareSource::title() const
{
    Q_D(const SoftwareSource);
    return d->title;
}

void SoftwareSource::setTitle(const QString &title)
{
    Q_D(SoftwareSource);

    if (d->title == title)
        return;

    d->title = title;
    Q_EMIT titleChanged();
}

QString SoftwareSource::section() const
{
    Q_D(const SoftwareSource);
    return d->section;
}

void SoftwareSource::setSection(const QString &section)
{
    Q_D(SoftwareSource);

    if (d->section == section)
        return;

    d->section = section;
    Q_EMIT sectionChanged();
}

bool SoftwareSource::isEnabled() const
{
    Q_D(const SoftwareSource);
    return d->enabled;
}

void SoftwareSource::setEnabled(bool flag)
{
    Q_D(SoftwareSource);

    if (d->enabled == flag)
        return;

    d->enabled = flag;
    Q_EMIT enabledChanged();
}

QUrl SoftwareSource::url() const
{
    Q_D(const SoftwareSource);
    return d->url;
}

void SoftwareSource::setUrl(const QUrl &url)
{
    Q_D(SoftwareSource);

    if (d->url == url)
        return;

    d->url = url;
    Q_EMIT urlChanged();
}

int SoftwareSource::priority() const
{
    Q_D(const SoftwareSource);
    return d->priority;
}

void SoftwareSource::setPriority(int prio)
{
    Q_D(SoftwareSource);

    if (d->priority == prio)
        return;

    d->priority = prio;
    Q_EMIT priorityChanged();
}

} // namespace AppCenter

} // namespace Liri
