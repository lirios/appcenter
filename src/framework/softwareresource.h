// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOFTWARERESOURCE_H
#define LIRIAPPCENTERSOFTWARERESOURCE_H

#include <QObject>
#include <QVariant>
#include <QVector>

#include <LiriAppCenter/Image>

namespace Liri {

namespace AppCenter {

class Rating;
class Review;
class SoftwareManager;
class SoftwareResourcePrivate;
class Transaction;

class LIRIAPPCENTER_EXPORT SoftwareResource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SoftwareManager *manager READ manager CONSTANT)
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString appId READ appId CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString summary READ summary CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(QUrl iconUrl READ iconUrl CONSTANT)
    Q_PROPERTY(QString packageName READ packageName CONSTANT)
    Q_PROPERTY(QString architecture READ architecture CONSTANT)
    Q_PROPERTY(QString license READ license CONSTANT)
    Q_PROPERTY(QString origin READ origin CONSTANT)
    Q_PROPERTY(QString category READ category CONSTANT)
    Q_PROPERTY(QUrl homepageUrl READ homepageUrl CONSTANT)
    Q_PROPERTY(QUrl bugtrackerUrl READ bugtrackerUrl CONSTANT)
    Q_PROPERTY(QUrl faqUrl READ faqUrl CONSTANT)
    Q_PROPERTY(QUrl helpUrl READ helpUrl CONSTANT)
    Q_PROPERTY(QUrl donationUrl READ donationUrl CONSTANT)
    Q_PROPERTY(QUrl translateUrl READ translateUrl CONSTANT)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString installedVersion READ installedVersion NOTIFY installedVersionChanged)
    Q_PROPERTY(QString availableVersion READ availableVersion NOTIFY availableVersionChanged)
    Q_PROPERTY(bool updatesAvailable READ updatesAvailable NOTIFY updatesAvailableChanged)
    Q_PROPERTY(quint64 downloadSize READ downloadSize CONSTANT)
    Q_PROPERTY(quint64 installedSize READ installedSize CONSTANT)
    Q_PROPERTY(quint64 size READ size CONSTANT)
    Q_PROPERTY(QString changeLog READ changeLog CONSTANT)
    Q_PROPERTY(bool installed READ isInstalled NOTIFY stateChanged)
    Q_PROPERTY(bool localized READ isLocalized CONSTANT)
    Q_PROPERTY(SoftwareResource::Kudos kudos READ kudos NOTIFY kudosChanged)
    Q_PROPERTY(uint kudosPercentage READ kudosPercentage NOTIFY kudosChanged)
    Q_PROPERTY(Rating *rating READ rating NOTIFY ratingChanged)
    Q_DECLARE_PRIVATE(SoftwareResource)
    Q_DISABLE_COPY(SoftwareResource)
public:
    enum Type {
        Addon,
        App,
        Codec,
        Driver,
        Firmware,
        Font,
        Generic,
        InputMethod,
        Localization,
        Runtime,
        Theme
    };
    Q_ENUM(Type)

    enum State {
        InstalledState,
        NotInstalledState,
        UpgradableState,
        BrokenState
    };
    Q_ENUM(State)

    enum Kudo {
        MyLanguageKudo = 1 << 0,
        RecentReleaseKudo = 1 << 1,
        FeaturedRecommendedKudo = 1 << 2,
        ModernToolkitKudo = 1 << 3,
        SearchProviderKudo = 1 << 4,
        InstallsUserDocsKudo = 1 << 5,
        AppMenuKudo = 1 << 6,
        UsesNotificationsKudo = 1 << 7,
        HasKeywordsKudo = 1 << 8,
        HasScreenshotsKudo = 1 << 9,
        PopularKudo = 1 << 10,
        HighContrastKudo = 1 << 11,
        HiDpiIconKudo = 1 << 12,
        SandboxedKudo = 1 << 13,
        SandboxedSecureKudo = 1 << 14,
    };
    Q_ENUM(Kudo)
    Q_DECLARE_FLAGS(Kudos, Kudo)
    Q_FLAG(Kudos)

    explicit SoftwareResource(SoftwareManager *manager,
                              QObject *parent = nullptr);
    ~SoftwareResource();

    SoftwareManager *manager() const;

    virtual Type type() const = 0;
    virtual State state() const = 0;

    virtual QString appId() const = 0;

    virtual QString name() const = 0;
    virtual QString summary() const = 0;
    virtual QString description() const = 0;

    virtual QString iconName() const;
    virtual QUrl iconUrl() const;

    virtual QString packageName() const = 0;
    virtual QString architecture() const = 0;

    virtual QString license() const = 0;

    virtual QString origin() const = 0;

    QString category() const;
    virtual QStringList categories() const = 0;

    virtual QUrl homepageUrl() const = 0;
    virtual QUrl bugtrackerUrl() const = 0;
    virtual QUrl faqUrl() const = 0;
    virtual QUrl helpUrl() const = 0;
    virtual QUrl donationUrl() const = 0;
    virtual QUrl translateUrl() const = 0;

    QString version() const;
    virtual QString installedVersion() const = 0;
    virtual QString availableVersion() const = 0;

    virtual bool updatesAvailable() const = 0;

    qint64 size() const;
    virtual quint64 downloadSize() const = 0;
    virtual quint64 installedSize() const = 0;

    virtual QString changeLog() const = 0;

    virtual QStringList iconNames() const = 0;
    virtual QVector<QUrl> iconUrls() const = 0;

    virtual QVector<Image> thumbnails() const = 0;
    virtual QVector<Image> screenshots() const = 0;

    bool isInstalled() const;
    virtual bool isLocalized() const = 0;

    Q_INVOKABLE virtual bool launch() const;
    Q_INVOKABLE virtual Transaction *install();
    Q_INVOKABLE virtual Transaction *uninstall();
    Q_INVOKABLE virtual Transaction *update();

    void addMetadata(const QString &key, const QVariant &value);
    QVariant getMetadata(const QString &key);

    Kudos kudos() const;

    Q_INVOKABLE bool hasKudo(Kudo kudo) const;

    void addKudo(Kudo kudo);
    void removeKudo(Kudo kudo);

    uint kudosPercentage() const;

    Rating *rating() const;

    QList<Review *> reviews() const;
    void fetchReviews();

    Q_INVOKABLE void submitReview(const QString &summary,
                                  const QString &description,
                                  int rating);

Q_SIGNALS:
    void stateChanged();
    void versionChanged();
    void installedVersionChanged();
    void availableVersionChanged();
    void updatesAvailableChanged();
    void installed();
    void updated();
    void uninstalled();
    void kudoAdded(SoftwareResource::Kudo kudo);
    void kudoRemoved(SoftwareResource::Kudo kudo);
    void kudosChanged();
    void ratingChanged();
    void reviewsFetched();
    void reviewSubmitFinished(Review *review);
    void reviewSubmitFailed(Review *review, const QString &errorMessage);
    void reviewReportFinished(Review *review);
    void reviewReportFailed(Review *review, const QString &errorMessage);
    void reviewUpVoteFinished(Review *review);
    void reviewUpVoteFailed(Review *review, const QString &errorMessage);
    void reviewDownVoteFinished(Review *review);
    void reviewDownVoteFailed(Review *review, const QString &errorMessage);
    void reviewDismissFinished(Review *review);
    void reviewDismissFailed(Review *review, const QString &errorMessage);
    void reviewRemoveFinished(Review *review);
    void reviewRemoveFailed(Review *review, const QString &errorMessage);

private:
    SoftwareResourcePrivate *const d_ptr;
};

typedef QVector<SoftwareResource *> SoftwareResources;

} // namespace AppCenter

} // namespace Liri

Q_DECLARE_OPERATORS_FOR_FLAGS(Liri::AppCenter::SoftwareResource::Kudos)
Q_DECLARE_METATYPE(Liri::AppCenter::SoftwareResource::Kudos)

#endif // LIRIAPPCENTERSOFTWARERESOURCE_H
