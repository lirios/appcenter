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

#ifndef LIRIAPPCENTERBACKENDPLUGIN_H
#define LIRIAPPCENTERBACKENDPLUGIN_H

#include <QObject>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class Backend;

class LIRIAPPCENTER_EXPORT BackendPlugin : public QObject
{
    Q_OBJECT
public:
    explicit BackendPlugin(QObject *parent = nullptr);

    virtual Backend *create() = 0;
};

} // namespace AppCenter

} // namespace Liri

#define BackendPlugin_iid "io.liri.AppCenter.BackendPlugin"

Q_DECLARE_INTERFACE(Liri::AppCenter::BackendPlugin, BackendPlugin_iid)

#endif // LIRIAPPCENTERBACKENDPLUGIN_H
