// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKSOURCE_H
#define LIRI_FLATPAKSOURCE_H

#include <QDir>

#include <LiriAppCenter/SoftwareSource>

#include "flatpakpluginbase.h"

class FlatpakSource : public Liri::AppCenter::SoftwareSource
{
    Q_OBJECT
public:
    explicit FlatpakSource(Liri::AppCenter::Backend *backend,
                           FlatpakInstallation *installation,
                           FlatpakRemote *remote,
                           QObject *parent = nullptr);
    ~FlatpakSource();

    QString name() const override;

    QString title() const override;
    void setTitle(const QString &value) override;

    QString comment() const override;
    void setComment(const QString &value) override;

    QString description() const override;
    void setDescription(const QString &value) override;

    QString section() const override;

    QString label() const override;

    bool isEnabled() const override;
    void setEnabled(bool value) override;

    bool gpgVerify() const override;
    void setGpgVerify(bool value) override;

    QUrl url() const override;
    void setUrl(const QUrl &value) override;

    QUrl iconUrl() const override;
    void setIconUrl(const QUrl &value) override;

    int priority() const override;
    void setPriority(int value) override;

    QVariantMap metadata() const override;

    FlatpakInstallation *installation() const;
    FlatpakRemote *remote() const;

    QDir appStreamDir() const;

private:
    FlatpakInstallation *m_installation = nullptr;
    FlatpakRemote *m_remote = nullptr;
    GCancellable *m_cancellable = nullptr;
    QString m_name;
    bool m_isUser = false;
    QVariantMap m_metadata;
};

#endif // LIRI_FLATPAKSOURCE_H
