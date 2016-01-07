/*
 * Papyros Software - The app store for Papyros
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
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
#include "xdg-app-plugin.h"

#include <QProcess>
#include <QStringList>
#include <QDebug>

#include "remote.h"
#include "application.h"

static void xdgAppChanged(GFileMonitor *monitor, GFile *child, GFile *other_file,
        GFileMonitorEvent event_type, XdgApp *xdgapp) {
    emit xdgapp->installationChanged();
}

XdgApp::XdgApp(QObject *parent)
    : QObject(parent)
{
    // Nothing needed here yet.
}

bool XdgApp::initialize()
{
    // Don't reinitialize
    if (m_installation != nullptr)
        return true;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    m_installation = xdg_app_installation_new_user(cancellable, &error);

    if (m_installation == nullptr) return false;

    m_monitor = xdg_app_installation_create_monitor(m_installation, cancellable, &error);

    if (m_monitor == nullptr) return false;

	g_signal_connect(m_monitor, "changed", G_CALLBACK(xdgAppChanged), this);

    return true;
}

QList<Remote *> XdgApp::listRemotes()
{
    QList<Remote *> remotes;

    if (!initialize())
        return remotes;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xremotes = xdg_app_installation_list_remotes(m_installation,
            cancellable, &error);

    if (xremotes == nullptr)
		return remotes;

	for (uint i = 0; i < xremotes->len; i++) {
		XdgAppRemote *xremote = (XdgAppRemote *) g_ptr_array_index(xremotes, i);

        remotes << new Remote(xremote, this);
	}

	return remotes;
}

QList<Application *> XdgApp::listInstalledApplications()
{
    QList<Application *> applications;

    if (!initialize())
        return applications;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xrefs = xdg_app_installation_list_installed_refs(m_installation,
            cancellable, &error);

    if (xrefs == nullptr)
		return applications;

	for (uint i = 0; i < xrefs->len; i++) {
		XdgAppInstalledRef *xref = (XdgAppInstalledRef *) g_ptr_array_index(xrefs, i);

        /*
    	 * Only show the current application in GNOME Software
    	 *
    	 * You can have multiple versions/branches of a particular app-id
    	 * installed but only one of them is "current" where this means:
    	 *  1) the default to launch unless you specify a version
    	 *  2) The one that gets its exported files exported
    	 */
        if (!xdg_app_installed_ref_get_is_current(xref) &&
    	    xdg_app_ref_get_kind(XDG_APP_REF(xref)) == XDG_APP_REF_KIND_APP) {
    		continue;
    	}

        applications << new Application(xref, Application::Installed, this);
	}

	return applications;
}
