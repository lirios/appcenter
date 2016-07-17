/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 * Copyright (C) 2015 Richard Hughes <richard@hughsie.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "flatpak-application.h"

#include "appstream/store.h"

FlatpakApplication::FlatpakApplication(FlatpakInstalledRef *app_ref, SoftwareBackend *backend)
        : Application(backend)
{
    m_state = Application::Installed;
    m_id = flatpak_ref_get_name(FLATPAK_REF(app_ref));
    m_branch = flatpak_ref_get_branch(FLATPAK_REF(app_ref));
    m_origin = flatpak_installed_ref_get_origin(app_ref);
    m_name = m_id;
    m_arch = flatpak_ref_get_arch(FLATPAK_REF(app_ref));

    m_currentCommit = flatpak_ref_get_commit(FLATPAK_REF(app_ref));
    m_latestCommit = flatpak_installed_ref_get_latest_commit(app_ref);

    if (m_branch.isEmpty())
        m_branch = "master";

    QString desktopId;

    switch (flatpak_ref_get_kind(FLATPAK_REF(app_ref))) {
    case FLATPAK_REF_KIND_APP:
        m_type = Application::App;

        desktopId = m_name + ".desktop";
        break;
    case FLATPAK_REF_KIND_RUNTIME:
        m_type = Application::Runtime;
        m_icon = QIcon::fromTheme("package-x-generic");
        m_summary = "Framework for applications";

        desktopId = m_name + ".runtime";
        break;
    default:
        // TODO: Handle errors here!
        break;
    }

    QString deployDir = flatpak_installed_ref_get_deploy_dir(app_ref);
    QString desktopPath = deployDir + "/files/share/applications";
    QString appdataPath = deployDir + "/files/share/appdata";

    Appstream::Store store;
    store.load(desktopPath);
    store.load(appdataPath);

    Appstream::Component component = store.componentById(desktopId);
    if (!component.isNull())
        refineFromAppstream(component);
}

FlatpakApplication::FlatpakApplication(Appstream::Component component, QString origin,
                               SoftwareBackend *backend)
        : Application(backend)
{
    m_state = Application::Available;
    m_origin = origin;

    QStringList bundle = component.m_bundle.split('/');

    m_branch = bundle[bundle.length() - 1];
    m_arch = bundle[bundle.length() - 2];

    refineFromAppstream(component);
}

void FlatpakApplication::install()
{
    /* install */
    // xref = flatpak_installation_install (plugin->priv->installation,
    // 				     gs_app_get_origin (app),
    // 				     FLATPAK_REF_KIND_APP,
    // 				     gs_app_get_id (app),
    // 				     gs_app_get_metadata_item (app, "XgdApp::arch"),
    // 				     gs_app_get_metadata_item (app, "XgdApp::branch"),
    // 				     gs_plugin_flatpak_progress_cb, &helper,
    // 				     cancellable, error);
}
