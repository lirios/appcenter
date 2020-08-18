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

#define UPDATE_FREQUENCY 150

static void progressCb(FlatpakTransactionProgress *progress,
                       gpointer data)
{
    auto *job = static_cast<FlatpakTransactionJob *>(data);
    if (!job)
        return;

    job->transaction()->setStatus(Liri::AppCenter::Transaction::Downloading);
    job->transaction()->setProgress(flatpak_transaction_progress_get_progress(progress));
}

static void newOpCb(FlatpakTransaction *transaction,
                    FlatpakTransactionOperation *operation,
                    FlatpakTransactionProgress *progress,
                    gpointer data)
{
    Q_UNUSED(transaction);
    Q_UNUSED(operation);

    auto *job = static_cast<FlatpakTransactionJob *>(data);
    if (!job)
        return;

    job->transaction()->setStatus(Liri::AppCenter::Transaction::Preparing);

    flatpak_transaction_progress_set_update_frequency(progress, UPDATE_FREQUENCY);
    g_signal_connect(progress, "changed", G_CALLBACK(progressCb), job);
}

static void readyCb(FlatpakTransaction *transaction,
                    gpointer data)
{
    Q_UNUSED(transaction);

    auto *job = static_cast<FlatpakTransactionJob *>(data);
    if (!job)
        return;

    job->transaction()->setStatus(Liri::AppCenter::Transaction::Committing);
}

static void doneCb(FlatpakTransaction *transaction,
                   FlatpakTransactionOperation *operation,
                   gchar *commit, gint result, gpointer data)
{
    Q_UNUSED(transaction);
    Q_UNUSED(operation);
    Q_UNUSED(commit);
    Q_UNUSED(result);

    auto *job = static_cast<FlatpakTransactionJob *>(data);
    if (!job)
        return;

    Q_EMIT job->succeeded();
}

static void errorCb(FlatpakTransaction *transaction,
                    FlatpakTransactionOperation *operation,
                    GError *error, gint details, gpointer data)
{
    Q_UNUSED(transaction);
    Q_UNUSED(operation);
    Q_UNUSED(details);

    auto *job = static_cast<FlatpakTransactionJob *>(data);
    if (!job)
        return;

    Q_EMIT job->failed(QString::fromUtf8(error->message));
}

FlatpakTransactionJob::FlatpakTransactionJob(FlatpakResource *app,
                                             Liri::AppCenter::Transaction *transaction,
                                             bool cancellable,
                                             QObject *parent)
    : QThread(parent)
    , m_app(app)
    , m_transaction(transaction)
{
    if (cancellable)
        m_cancellable = g_cancellable_new();
}

FlatpakTransactionJob::~FlatpakTransactionJob()
{
    if (m_cancellable)
        g_object_unref(m_cancellable);
}

Liri::AppCenter::SoftwareResource *FlatpakTransactionJob::resource() const
{
    return m_app;
}

Liri::AppCenter::Transaction *FlatpakTransactionJob::transaction() const
{
    return m_transaction;
}

bool FlatpakTransactionJob::isCancellable() const
{
    return m_cancellable;
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
    g_signal_connect(transaction, "new-operation", G_CALLBACK(newOpCb), this);
    g_signal_connect(transaction, "ready", G_CALLBACK(readyCb), this);
    g_signal_connect(transaction, "operation-done", G_CALLBACK(doneCb), this);
    g_signal_connect(transaction, "operation-error", G_CALLBACK(errorCb), this);

    // Add the operation
    if (m_transaction->type() == Liri::AppCenter::Transaction::Install) {
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
    } else if (m_transaction->type() == Liri::AppCenter::Transaction::Uninstall) {
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
    } else if (m_transaction->type() == Liri::AppCenter::Transaction::Update) {
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
    if (m_transaction->type() == Liri::AppCenter::Transaction::Install ||
            m_transaction->type() == Liri::AppCenter::Transaction::Update) {
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
    } else if (m_transaction->type() == Liri::AppCenter::Transaction::Uninstall) {
        m_app->updateFromRef(nullptr);
        m_app->updateFromInstalledRef(nullptr);
    }
}

void FlatpakTransactionJob::cancel()
{
    if (m_cancellable)
        g_cancellable_cancel(m_cancellable);
    Q_EMIT cancelled();
}
