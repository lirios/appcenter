// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
