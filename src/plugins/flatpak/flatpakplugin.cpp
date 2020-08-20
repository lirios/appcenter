// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "flatpakplugin.h"

FlatpakPlugin::FlatpakPlugin(QObject *parent)
    : Liri::AppCenter::BackendPlugin(parent)
{
}

Liri::AppCenter::Backend *FlatpakPlugin::create(Liri::AppCenter::SoftwareManager *manager)
{
    return new FlatpakBackend(manager);
}
