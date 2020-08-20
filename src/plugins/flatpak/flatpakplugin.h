// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKPLUGIN_H
#define LIRI_FLATPAKPLUGIN_H

#include <LiriAppCenter/BackendPlugin>

#include "flatpakbackend.h"

class FlatpakPlugin : public Liri::AppCenter::BackendPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID BackendPlugin_iid FILE "flatpak.json")
    Q_INTERFACES(Liri::AppCenter::BackendPlugin)
public:
    explicit FlatpakPlugin(QObject *parent = nullptr);

    Liri::AppCenter::Backend *create(Liri::AppCenter::SoftwareManager *manager) override;
};

#endif // LIRI_FLATPAKPLUGIN_H
