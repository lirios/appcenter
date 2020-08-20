// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "softwareresource_p.h"
#include "transaction.h"

namespace Liri {

namespace AppCenter {

SoftwareResourcePrivate::SoftwareResourcePrivate()
{
}

SoftwareResource::SoftwareResource(QObject *parent)
    : QObject(parent)
    , d_ptr(new SoftwareResourcePrivate())
{
    qRegisterMetaType<SoftwareResource::Kudos>("SoftwareResource::Kudos");
}

SoftwareResource::~SoftwareResource()
{
    delete d_ptr;
}

QString SoftwareResource::iconName() const
{
    if (iconNames().size() > 0) {
        for (const auto &name : iconNames()) {
            if (!name.isEmpty())
                return name;
        }
    }
    return QLatin1String("package-x-generic");
}

QUrl SoftwareResource::iconUrl() const
{
    if (iconUrls().size() > 0) {
        for (const auto &url : iconUrls()) {
            if (url.isValid())
                return url;
        }
    }
    return QUrl();
}

QString SoftwareResource::category() const
{
    return categories().join(QString::fromUtf8(" → "));
}

QString SoftwareResource::version() const
{
    if (state() == BrokenState)
        return QString();
    else if (state() == InstalledState)
        return installedVersion();
    return availableVersion();
}

bool SoftwareResource::launch() const
{
    return false;
}

Transaction *SoftwareResource::install()
{
    return nullptr;
}

Transaction *SoftwareResource::uninstall()
{
    return nullptr;
}

Transaction *SoftwareResource::update()
{
    return nullptr;
}

void SoftwareResource::addMetadata(const QString &key, const QVariant &value)
{
    Q_D(SoftwareResource);
    d->metadata.insert(key, value);
}

QVariant SoftwareResource::getMetadata(const QString &key)
{
    Q_D(SoftwareResource);
    return d->metadata.value(key);
}

SoftwareResource::Kudos SoftwareResource::kudos() const
{
    Q_D(const SoftwareResource);
    return d->kudos;
}

// FIXME: For some reason we can't test the flag from QML so we need this method
bool SoftwareResource::hasKudo(SoftwareResource::Kudo kudo) const
{
    Q_D(const SoftwareResource);
    return d->kudos.testFlag(kudo);
}

void SoftwareResource::addKudo(SoftwareResource::Kudo kudo)
{
    Q_D(SoftwareResource);

    bool changed = false;

    if (!d->kudos.testFlag(kudo)) {
        d->kudos.setFlag(kudo, true);
        changed = true;
        Q_EMIT kudoAdded(kudo);
    }

    if (kudo == SoftwareResource::SandboxedSecureKudo &&
            !d->kudos.testFlag(SoftwareResource::SandboxedKudo)) {
        d->kudos.setFlag(SoftwareResource::SandboxedKudo, true);
        changed = true;
        Q_EMIT kudoAdded(SoftwareResource::SandboxedKudo);
    }

    if (changed)
        Q_EMIT kudosChanged();
}

void SoftwareResource::removeKudo(SoftwareResource::Kudo kudo)
{
    Q_D(SoftwareResource);

    bool changed = false;

    if (d->kudos.testFlag(kudo)) {
        d->kudos.setFlag(kudo, false);
        changed = true;
        Q_EMIT kudoRemoved(kudo);
    }

    if (kudo == SoftwareResource::SandboxedKudo &&
            d->kudos.testFlag(SoftwareResource::SandboxedSecureKudo)) {
        d->kudos.setFlag(SoftwareResource::SandboxedSecureKudo, false);
        changed = true;
        Q_EMIT kudoRemoved(SoftwareResource::SandboxedSecureKudo);
    }

    if (changed)
        Q_EMIT kudosChanged();
}

uint SoftwareResource::kudosPercentage() const
{
    Q_D(const SoftwareResource);

    // Algorithm based on gnome-software

    uint percentage = 0;

    if (d->kudos.testFlag(SoftwareResource::MyLanguageKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::RecentReleaseKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::FeaturedRecommendedKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::ModernToolkitKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::SearchProviderKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::InstallsUserDocsKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::UsesNotificationsKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::HasKeywordsKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::HasScreenshotsKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::HighContrastKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::HiDpiIconKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::SandboxedKudo))
        percentage += 20;
    if (d->kudos.testFlag(SoftwareResource::SandboxedSecureKudo))
        percentage += 20;

    // Popular apps should be at least 50%
    if (d->kudos.testFlag(SoftwareResource::PopularKudo))
        percentage = qMax<uint>(percentage, 50);

    return qMin<uint>(percentage, 100);
}

} // namespace AppCenter

} // namespace Liri
