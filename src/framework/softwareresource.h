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

#ifndef LIRIAPPCENTERSOFTWARERESOURCE_H
#define LIRIAPPCENTERSOFTWARERESOURCE_H

#include <QObject>
#include <QVariant>
#include <QVector>

#include <LiriAppCenter/Image>

namespace Liri {

namespace AppCenter {

class SoftwareResourcePrivate;
class Transaction;

class LIRIAPPCENTER_EXPORT SoftwareResource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
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
    Q_PROPERTY(QString changeLog READ changeLog CONSTANT)
    Q_PROPERTY(bool localized READ isLocalized CONSTANT)
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
    Q_ENUM(Type);

    enum State {
        InstalledState,
        NotInstalledState,
        UpgradableState,
        BrokenState
    };
    Q_ENUM(State);

    explicit SoftwareResource(QObject *parent = nullptr);
    ~SoftwareResource();

    virtual Type type() const = 0;
    virtual State state() const = 0;

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

    virtual quint64 downloadSize() const = 0;
    virtual quint64 installedSize() const = 0;

    virtual QString changeLog() const = 0;

    virtual QStringList iconNames() const = 0;
    virtual QVector<QUrl> iconUrls() const = 0;

    virtual QVector<Image> thumbnails() const = 0;
    virtual QVector<Image> screenshots() const = 0;

    virtual bool isLocalized() const = 0;

    Q_INVOKABLE virtual bool launch() const;
    Q_INVOKABLE virtual Transaction *install();
    Q_INVOKABLE virtual Transaction *uninstall();
    Q_INVOKABLE virtual Transaction *update();

    void addMetadata(const QString &key, const QVariant &value);
    QVariant getMetadata(const QString &key);

Q_SIGNALS:
    void stateChanged();
    void versionChanged();
    void installedVersionChanged();
    void availableVersionChanged();
    void updatesAvailableChanged();
    void installed();
    void updated();
    void uninstalled();

private:
    SoftwareResourcePrivate *const d_ptr;
};

typedef QVector<SoftwareResource *> SoftwareResources;

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOFTWARERESOURCE_H
