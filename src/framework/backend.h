// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERBACKEND_H
#define LIRIAPPCENTERBACKEND_H

#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SoftwareSource>

namespace Liri {

namespace AppCenter {

class SoftwareManager;

class LIRIAPPCENTER_EXPORT Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(SoftwareManager *manager, QObject *parent = nullptr);
    virtual ~Backend();

    virtual void initialize() = 0;

    virtual void listSources() = 0;
    virtual void listAvailableApps() = 0;
    virtual void listInstalledApps() = 0;
    virtual void checkForUpdates() = 0;

    virtual QVector<SoftwareResource *> updates() const = 0;

    virtual bool addSource(const QString &source) = 0;
    virtual bool removeSource(SoftwareSource *source) = 0;

    virtual SoftwareResource *installAppFromPath(const QString &filePath) = 0;

Q_SIGNALS:
    void updatesAvailable(uint count);
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERBACKEND_H
