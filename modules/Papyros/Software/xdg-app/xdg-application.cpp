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

#include "xdg-application.h"

#include "appstream/store.h"

XdgApplication::XdgApplication(XdgAppInstalledRef *app_ref, State state, QObject *parent)
    : Application(parent)
{
    m_state = state;
    m_branch = xdg_app_ref_get_branch(XDG_APP_REF(app_ref));
    m_origin = xdg_app_installed_ref_get_origin(app_ref);
    m_name = xdg_app_ref_get_name(XDG_APP_REF(app_ref));
    m_arch = xdg_app_ref_get_arch(XDG_APP_REF(app_ref));

    QString desktopId;

    switch (xdg_app_ref_get_kind(XDG_APP_REF(app_ref))) {
    case XDG_APP_REF_KIND_APP:
        m_type = Application::App;

        desktopId = m_name + ".desktop";
        break;
    case XDG_APP_REF_KIND_RUNTIME:
        m_type = Application::Runtime;
        m_iconName = "application-x-executable";
        m_summary = "Framework for applications";

        desktopId = m_name + ".runtime";
        break;
    default:
        // TODO: Handle errors here!
        break;
    }

    QString deployDir = xdg_app_installed_ref_get_deploy_dir(app_ref);
    QString desktopPath = deployDir + "/files/share/applications";
    QString appdataPath = deployDir + "/files/share/appdata";

    Appstream::Store store;
    store.load(desktopPath);
    store.load(appdataPath);

    Appstream::Component component = store.componentById(desktopId);
    if (!component.isNull())
        refineFromAppstream(component);
}

void XdgApplication::install()
{
    /* install */
	// xref = xdg_app_installation_install (plugin->priv->installation,
	// 				     gs_app_get_origin (app),
	// 				     XDG_APP_REF_KIND_APP,
	// 				     gs_app_get_id (app),
	// 				     gs_app_get_metadata_item (app, "XgdApp::arch"),
	// 				     gs_app_get_metadata_item (app, "XgdApp::branch"),
	// 				     gs_plugin_xdg_app_progress_cb, &helper,
	// 				     cancellable, error);
}
