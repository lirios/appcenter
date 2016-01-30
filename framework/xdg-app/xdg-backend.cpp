/*
 * Papyros Software - The app store for Papyros
 *
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
#include "xdg-backend.h"

#include <QProcess>
#include <QStringList>
#include <QDebug>

#include "xdg-remote.h"
#include "xdg-application.h"
#include "utils.h"

#include "appstream/store.h"

struct XdgAppHelper
{
    XdgApplication *app;
};

static void xdgAppChanged(GFileMonitor *monitor, GFile *child, GFile *other_file,
                          GFileMonitorEvent event_type, XdgAppBackend *xdgapp)
{
    emit xdgapp->updated();
}

static void xdgAppProgressCallback(const char *rawStatus, uint progress, bool estimating,
                                   void *userData)
{
    QString status = rawStatus;
    XdgAppHelper *helper = static_cast<XdgAppHelper *>(userData);

    Q_ASSERT(helper->app);

    qDebug() << "Progress callback for" << helper->app->m_id << status << progress << estimating;
    // TODO: Do something more!
}

XdgAppBackend::XdgAppBackend(QObject *parent) : SoftwareBackend(parent)
{
    // Nothing needed here yet.
}

void XdgAppBackend::initialize() throw(InitializationFailedException)
{
    // Don't reinitialize
    if (m_installation != nullptr)
        return;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    m_installation = xdg_app_installation_new_user(cancellable, &error);

    if (m_installation == nullptr)
        throw InitializationFailedException(error);

    m_monitor = xdg_app_installation_create_monitor(m_installation, cancellable, &error);

    if (m_monitor == nullptr)
        throw InitializationFailedException(error);

    g_signal_connect(m_monitor, "changed", G_CALLBACK(xdgAppChanged), this);
}

QList<SoftwareSource *> XdgAppBackend::listSources()
{
    initialize();

    QList<SoftwareSource *> sources;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xremotes =
            xdg_app_installation_list_remotes(m_installation, cancellable, &error);

    if (xremotes == nullptr)
        return sources;

    G_FOREACH (XdgAppRemote *xremote, xremotes) {
        sources << new Remote(xremote, this);
    }

    return sources;
}

QList<Application *> XdgAppBackend::listAvailableApplications()
{
    initialize();

    QList<Application *> applications;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xremotes =
            xdg_app_installation_list_remotes(m_installation, cancellable, &error);

    if (xremotes == nullptr)
        return applications;

    G_FOREACH (XdgAppRemote *xremote, xremotes) {
        /* add the new AppStream repo to the shared store */
        g_autoptr(GFile) file = xdg_app_remote_get_appstream_dir(xremote, NULL);
        QString appstreamFilename = g_file_get_path(file);

        QString origin = xdg_app_remote_get_name(xremote);

        qDebug() << "Using AppStream metadata found at: " << appstreamFilename << origin;

        Appstream::Store store;
        store.load(appstreamFilename);

        foreach (Appstream::Component component, store.allComponents()) {
            applications << new XdgApplication(component, origin, this);
        }
    }

    return applications;
}

QList<Application *> XdgAppBackend::listInstalledApplications()
{
    initialize();

    QList<Application *> applications;

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xrefs =
            xdg_app_installation_list_installed_refs(m_installation, cancellable, &error);

    if (xrefs == nullptr)
        return applications;

    G_FOREACH (XdgAppInstalledRef *xref, xrefs) {
        /*
         * Only show the current application in GNOME Software
         *
         * You can have multiple versions/branches of a particular app-id
         * installed but only one of them is "current" where this means:
         *  1) the default to launch unless you specify a version
         *  2) The one that gets its exported files exported
         */
        qDebug() << xdg_app_installed_ref_get_is_current(xref);
        if (!xdg_app_installed_ref_get_is_current(xref) &&
            xdg_app_ref_get_kind(XDG_APP_REF(xref)) == XDG_APP_REF_KIND_APP) {
            qDebug() << "Skipping!";
            continue;
        }

        applications << new XdgApplication(xref, this);
    }

    return applications;
}

bool XdgAppBackend::launchApplication(const Application *app)
{
    initialize();

    const XdgApplication *xapp = qobject_cast<const XdgApplication *>(app);

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    return xdg_app_installation_launch(m_installation, qPrintable(xapp->m_id), NULL,
                                       qPrintable(xapp->m_branch), NULL, cancellable, &error);
}

bool XdgAppBackend::downloadUpdates()
{
    initialize();

    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    /* get all the updates available from all remotes */
    g_autoptr(GPtrArray) xrefs = xdg_app_installation_list_installed_refs_for_update(
            m_installation, cancellable, &error);

    if (xrefs == nullptr)
        throw GLibException("Unable to get list of updates", error);

    if (xrefs->len == 0)
        return false;

    qDebug() << "Downloading" << xrefs->len << "updates";

    G_FOREACH (XdgAppInstalledRef *xref, xrefs) {
        XdgApplication *app = new XdgApplication(xref, this);

        XdgAppHelper helper = {app};

        qDebug() << "Updating" << xdg_app_ref_get_name(XDG_APP_REF(xref));
        XdgAppInstalledRef *xref2 = xdg_app_installation_update(
                m_installation, XDG_APP_UPDATE_FLAGS_NO_DEPLOY,
                xdg_app_ref_get_kind(XDG_APP_REF(xref)), xdg_app_ref_get_name(XDG_APP_REF(xref)),
                xdg_app_ref_get_arch(XDG_APP_REF(xref)), xdg_app_ref_get_branch(XDG_APP_REF(xref)),
                (XdgAppProgressCallback) xdgAppProgressCallback, &helper, cancellable, &error);
        if (xref2 == nullptr) {
            QString what = "Unable to download update for " +
                           QString(xdg_app_ref_get_name(XDG_APP_REF(xref)));
            throw GLibException(qPrintable(what), error);
        }
    }

    return true;
}

bool XdgAppBackend::refreshAvailableApplications()
{
    initialize();

    // TODO: Do something with these
    GCancellable *cancellable = nullptr;
    GError *error = nullptr;

    g_autoptr(GPtrArray) xremotes =
            xdg_app_installation_list_remotes(m_installation, cancellable, &error);

    if (xremotes == nullptr)
        return false;

    G_FOREACH (XdgAppRemote *xremote, xremotes) {
        g_autoptr(GError) error_local = NULL;

        /* is the timestamp new enough */
        // TODO: Figure this out
        // g_autoptr(GFile) file_timestamp = xdg_app_remote_get_appstream_timestamp(xremote, NULL);
        // uint file_age = gs_utils_get_file_age(file_timestamp);
        // if (file_age < cache_age) {
        //     QString filename = g_file_get_path(file_timestamp);
        //     qDebug() << filename << "is only" << file_age << "seconds old, so ignoring refresh";
        //     continue;
        // }

        /* download new data */
        bool result = xdg_app_installation_update_appstream_sync(
                m_installation, xdg_app_remote_get_name(xremote), NULL, /* arch */
                NULL,                                                   /* out_changed */
                cancellable, &error_local);
        if (!result) {
            if (g_error_matches(error_local, G_IO_ERROR, G_IO_ERROR_FAILED)) {
                qDebug() << "Failed to get AppStream metadata:" << error_local->message;
                continue;
            }

            qWarning() << "Failed to get AppStream metadata, aborting:" << error_local->message;
            return false;
        }
    }

    qDebug() << "Finished updating applications!";

    emit availableApplicationsChanged();
    return true;
}
