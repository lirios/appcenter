// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_RESOURCEPROXY_H
#define LIRI_APPCENTER_RESOURCEPROXY_H

#include <QObject>

#include <LiriAppCenter/SoftwareResource>

namespace Liri {

namespace AppCenter {

class ResourceProxyPrivate;
class SoftwareSource;
class SourcesModel;

class LIRIAPPCENTER_EXPORT ResourceProxy : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ResourceProxy)
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(SoftwareSource *defaultSource READ defaultSource WRITE setDefaultSource NOTIFY defaultSourceChanged)
    Q_PROPERTY(int defaultSourceIndex READ defaultSourceIndex NOTIFY defaultSourceChanged)
    Q_PROPERTY(SoftwareResource *selectedResource READ selectedResource NOTIFY selectedResourceChanged)
    Q_PROPERTY(SourcesModel *sourcesModel READ sourcesModel CONSTANT)
    Q_PROPERTY(SoftwareResource::Type type READ type NOTIFY dataChanged)
    Q_PROPERTY(SoftwareResource::State state READ state NOTIFY dataChanged)
    Q_PROPERTY(QString name READ name NOTIFY dataChanged)
    Q_PROPERTY(QString summary READ summary NOTIFY dataChanged)
    Q_PROPERTY(QString description READ description NOTIFY dataChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY dataChanged)
    Q_PROPERTY(QUrl iconUrl READ iconUrl NOTIFY dataChanged)
    Q_PROPERTY(QString packageName READ packageName NOTIFY dataChanged)
    Q_PROPERTY(QString architecture READ architecture NOTIFY dataChanged)
    Q_PROPERTY(QString license READ license NOTIFY dataChanged)
    Q_PROPERTY(QString category READ category NOTIFY dataChanged)
    Q_PROPERTY(QUrl homepageUrl READ homepageUrl NOTIFY dataChanged)
    Q_PROPERTY(QUrl bugtrackerUrl READ bugtrackerUrl NOTIFY dataChanged)
    Q_PROPERTY(QUrl faqUrl READ faqUrl NOTIFY dataChanged)
    Q_PROPERTY(QUrl helpUrl READ helpUrl NOTIFY dataChanged)
    Q_PROPERTY(QUrl donationUrl READ donationUrl NOTIFY dataChanged)
    Q_PROPERTY(QUrl translateUrl READ translateUrl NOTIFY dataChanged)
    Q_PROPERTY(QString version READ version NOTIFY dataChanged)
    Q_PROPERTY(QString installedVersion READ installedVersion NOTIFY dataChanged)
    Q_PROPERTY(QString availableVersion READ availableVersion NOTIFY dataChanged)
    Q_PROPERTY(bool updatesAvailable READ updatesAvailable NOTIFY dataChanged)
    Q_PROPERTY(quint64 downloadSize READ downloadSize NOTIFY dataChanged)
    Q_PROPERTY(quint64 installedSize READ installedSize NOTIFY dataChanged)
    Q_PROPERTY(quint64 size READ size NOTIFY dataChanged)
    Q_PROPERTY(QString changeLog READ changeLog NOTIFY dataChanged)
    Q_PROPERTY(bool installed READ isInstalled NOTIFY dataChanged)
    Q_PROPERTY(bool localized READ isLocalized NOTIFY dataChanged)
    Q_PROPERTY(SoftwareResource::Kudos kudos READ kudos NOTIFY dataChanged)
    Q_PROPERTY(uint kudosPercentage READ kudosPercentage NOTIFY dataChanged)
    Q_PROPERTY(Rating *rating READ rating NOTIFY dataChanged)
public:
    explicit ResourceProxy(QObject *parent = nullptr);
    ~ResourceProxy();

    QString id() const;

    SoftwareSource *defaultSource() const;
    void setDefaultSource(SoftwareSource *source);

    int defaultSourceIndex() const;

    QList<SoftwareSource *> sources() const;
    SourcesModel *sourcesModel() const;

    SoftwareResource *selectedResource() const;

    bool hasResource(SoftwareResource *resource) const;
    QList<SoftwareResource *> resources() const;

    void addResource(SoftwareResource *resource);
    bool removeResource(SoftwareResource *resource);

    SoftwareResource::Type type() const;
    SoftwareResource::State state() const;
    QString name() const;
    QString summary() const;
    QString description() const;
    QString iconName() const;
    QUrl iconUrl() const;
    QString packageName() const;
    QString architecture() const;
    QString license() const;
    QString category() const;
    QUrl homepageUrl() const;
    QUrl bugtrackerUrl() const;
    QUrl faqUrl() const;
    QUrl helpUrl() const;
    QUrl donationUrl() const;
    QUrl translateUrl() const;
    QString version() const;
    QString installedVersion() const;
    QString availableVersion() const;
    bool updatesAvailable() const;
    quint64 downloadSize() const;
    quint64 installedSize() const;
    quint64 size() const;
    QString changeLog() const;
    bool isInstalled() const;
    bool isLocalized() const;
    SoftwareResource::Kudos kudos() const;
    uint kudosPercentage() const;
    Rating *rating() const;

    Q_INVOKABLE bool hasKudo(SoftwareResource::Kudo kudo) const;

    Q_INVOKABLE virtual bool launch() const;
    Q_INVOKABLE virtual Liri::AppCenter::Transaction *install();
    Q_INVOKABLE virtual Liri::AppCenter::Transaction *uninstall();
    Q_INVOKABLE virtual Liri::AppCenter::Transaction *update();

    bool operator==(const ResourceProxy &other);

Q_SIGNALS:
    void defaultSourceChanged();
    void selectedResourceChanged();
    void dataChanged();

private:
    ResourceProxyPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_RESOURCEPROXY_H
