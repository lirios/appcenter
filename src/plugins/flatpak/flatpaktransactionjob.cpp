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

#include "flatpakplugin.h"
#include "flatpaktransactionjob.h"

static void flatpakProgress(const gchar *stats, guint progress, gboolean estimating, gpointer userData)
{
    Q_UNUSED(stats);
    Q_UNUSED(estimating);

    FlatpakTransactionJob *job = static_cast<FlatpakTransactionJob *>(userData);
    if (!job)
        return;

    job->setProgress(progress);
}

FlatpakTransactionJob::FlatpakTransactionJob(FlatpakResource *app, Liri::AppCenter::Transaction::Type type, QObject *parent)
    : QThread(parent)
    , m_app(app)
    , m_type(type)
    , m_cancellable(g_cancellable_new())
{
}

FlatpakTransactionJob::~FlatpakTransactionJob()
{
    g_object_unref(m_cancellable);
}

int FlatpakTransactionJob::progress() const
{
    return m_progress;
}

void FlatpakTransactionJob::setProgress(int progress)
{
    if (m_progress == progress)
        return;

    m_progress = progress;
    Q_EMIT progressChanged(progress);
}

void FlatpakTransactionJob::run()
{
    g_autoptr(GError) error = nullptr;

    if (m_type == Liri::AppCenter::Transaction::Install) {
        FlatpakInstalledRef *ref =
                flatpak_installation_install(m_app->installation(),
                                             m_app->origin().toUtf8().constData(),
                                             m_app->kind(),
                                             m_app->packageName().toUtf8().constData(),
                                             m_app->architecture().toUtf8().constData(),
                                             m_app->branch().toUtf8().constData(),
                                             flatpakProgress, this,
                                             m_cancellable, &error);
        if (!ref) {
            qCWarning(lcFlatpakBackend, "Failed to install \"%s\": %s",
                      m_app->name().toUtf8().constData(),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }

        m_app->updateFromRef(FLATPAK_REF(ref));
        m_app->updateFromInstalledRef(ref);
    } else if (m_type == Liri::AppCenter::Transaction::Uninstall) {
        if (!flatpak_installation_uninstall(m_app->installation(),
                                            m_app->kind(),
                                            m_app->packageName().toUtf8().constData(),
                                            m_app->architecture().toUtf8().constData(),
                                            m_app->branch().toUtf8().constData(),
                                            flatpakProgress, this,
                                            m_cancellable, &error)) {
            qCWarning(lcFlatpakBackend, "Failed to uninstall \"%s\": %s",
                      m_app->name().toUtf8().constData(),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }

        m_app->updateFromRef(nullptr);
        m_app->updateFromInstalledRef(nullptr);
    } else if (m_type == Liri::AppCenter::Transaction::Update) {
        FlatpakInstalledRef *ref =
                flatpak_installation_update(m_app->installation(),
                                            FLATPAK_UPDATE_FLAGS_NONE,
                                            m_app->kind(),
                                            m_app->packageName().toUtf8().constData(),
                                            m_app->architecture().toUtf8().constData(),
                                            m_app->branch().toUtf8().constData(),
                                            flatpakProgress, this,
                                            m_cancellable, &error);
        if (!ref) {
            qCWarning(lcFlatpakBackend, "Failed to update \"%s\": %s",
                      m_app->name().toUtf8().constData(),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }

        m_app->updateFromRef(FLATPAK_REF(ref));
        m_app->updateFromInstalledRef(ref);
    }

    // Job finished
    setProgress(100);
    Q_EMIT succeeded();
}

void FlatpakTransactionJob::cancel()
{
    g_cancellable_cancel(m_cancellable);
    Q_EMIT cancelled();
}
