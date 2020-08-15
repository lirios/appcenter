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

static void flatpakNewOperation(FlatpakTransaction *transaction,
                                FlatpakTransactionOperation *operation,
                                FlatpakTransactionProgress *progress,
                                gpointer data)
{
    Q_UNUSED(transaction);
    Q_UNUSED(operation);

    auto *job = static_cast<FlatpakTransactionJob *>(data);
    if (!job)
        return;

    job->setProgress(flatpak_transaction_progress_get_progress(progress));
}

FlatpakTransactionJob::FlatpakTransactionJob(FlatpakResource *app,
                                             Liri::AppCenter::Transaction::Type type,
                                             bool cancellable,
                                             QObject *parent)
    : QThread(parent)
    , m_app(app)
    , m_type(type)
{
    if (cancellable)
        m_cancellable = g_cancellable_new();
}

FlatpakTransactionJob::~FlatpakTransactionJob()
{
    if (m_cancellable)
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

    // Create the transaction
    g_autoptr(FlatpakTransaction) transaction =
            flatpak_transaction_new_for_installation(m_app->installation(),
                                                     m_cancellable,
                                                     &error);
    if (!transaction) {
        qCWarning(lcFlatpakBackend,
                  "Failed to create a transaction for \"%s\": %s",
                  qPrintable(m_app->name()),
                  error->message);
        Q_EMIT failed(QString::fromUtf8(error->message));
        return;
    }

    // Update the progress
    g_signal_connect(transaction, "new-operation", G_CALLBACK(flatpakNewOperation), this);

    // Add the operation
    if (m_type == Liri::AppCenter::Transaction::Install) {
        if (!flatpak_transaction_add_install(transaction,
                                             m_app->origin().toUtf8().constData(),
                                             m_app->ref().toUtf8().constData(),
                                             nullptr,
                                             &error)) {
            qCWarning(lcFlatpakBackend,
                      "Failed to add installation of \"%s\" to the transaction: %s",
                      qPrintable(m_app->name()),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }
    } else if (m_type == Liri::AppCenter::Transaction::Uninstall) {
        if (!flatpak_transaction_add_uninstall(transaction,
                                               m_app->ref().toUtf8().constData(),
                                               &error)) {
            qCWarning(lcFlatpakBackend,
                      "Failed to add uninstallation of \"%s\" to the transaction: %s",
                      qPrintable(m_app->name()),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }
    } else if (m_type == Liri::AppCenter::Transaction::Update) {
        if (!flatpak_transaction_add_update(transaction,
                                            m_app->ref().toUtf8().constData(),
                                            nullptr, nullptr, &error)) {
            qCWarning(lcFlatpakBackend,
                      "Failed to add update of \"%s\" to the transaction: %s",
                      qPrintable(m_app->name()),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }
    }

    // Run the transaction
    if (!flatpak_transaction_run(transaction, m_cancellable, &error)) {
        qCWarning(lcFlatpakBackend,
                  "Failed to run the transaction for \"%s\": %s",
                  qPrintable(m_app->name()),
                  error->message);
        Q_EMIT failed(QString::fromUtf8(error->message));
        return;
    }

    // Update resource
    if (m_type == Liri::AppCenter::Transaction::Install ||
            m_type == Liri::AppCenter::Transaction::Update) {
        auto *installedRef =
                flatpak_installation_get_installed_ref(
                    m_app->installation(),
                    m_app->kind(),
                    m_app->packageName().toUtf8().constData(),
                    m_app->architecture().toUtf8().constData(),
                    m_app->branch().toUtf8().constData(),
                    m_cancellable,
                    &error);
        if (!installedRef) {
            qCWarning(lcFlatpakBackend, "Failed to refresh \"%s\": %s",
                      qPrintable(m_app->name()),
                      error->message);
            Q_EMIT failed(QString::fromUtf8(error->message));
            return;
        }

        m_app->updateFromRef(FLATPAK_REF(installedRef));
        m_app->updateFromInstalledRef(installedRef);
    } else if (m_type == Liri::AppCenter::Transaction::Uninstall) {
        m_app->updateFromRef(nullptr);
        m_app->updateFromInstalledRef(nullptr);
    }

    // Job finished
    setProgress(100);
    Q_EMIT succeeded();
}

void FlatpakTransactionJob::cancel()
{
    if (m_cancellable)
        g_cancellable_cancel(m_cancellable);
    Q_EMIT cancelled();
}
