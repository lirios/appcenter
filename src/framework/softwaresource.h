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

#ifndef LIRIAPPCENTERSOFTWARESOURCE_H
#define LIRIAPPCENTERSOFTWARESOURCE_H

#include <QObject>
#include <QUrl>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class Backend;
class SoftwareSourcePrivate;

class LIRIAPPCENTER_EXPORT SoftwareSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Backend *backend READ backend CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString section READ section CONSTANT)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool gpgVerify READ gpgVerify WRITE setGpgVerify NOTIFY gpgVerifyChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QUrl iconUrl READ iconUrl WRITE setIconUrl NOTIFY iconUrlChanged)
    Q_PROPERTY(int priority READ priority WRITE setPriority NOTIFY priorityChanged)
    Q_DECLARE_PRIVATE(SoftwareSource)
    Q_DISABLE_COPY(SoftwareSource)
public:
    explicit SoftwareSource(Backend *backend, QObject *parent = nullptr);
    ~SoftwareSource();

    Backend *backend() const;

    virtual QString name() const = 0;

    virtual QString title() const = 0;
    virtual void setTitle(const QString &title) = 0;

    virtual QString comment() const = 0;
    virtual void setComment(const QString &comment) = 0;

    virtual QString description() const = 0;
    virtual void setDescription(const QString &description) = 0;

    virtual QString section() const = 0;

    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool flag) = 0;

    virtual bool gpgVerify() const = 0;
    virtual void setGpgVerify(bool flag) = 0;

    virtual QUrl url() const = 0;
    virtual void setUrl(const QUrl &url) = 0;

    virtual QUrl iconUrl() const = 0;
    virtual void setIconUrl(const QUrl &url) = 0;

    virtual int priority() const = 0;
    virtual void setPriority(int prio) = 0;

Q_SIGNALS:
    void titleChanged();
    void commentChanged();
    void descriptionChanged();
    void enabledChanged();
    void gpgVerifyChanged();
    void urlChanged();
    void iconUrlChanged();
    void priorityChanged();

private:
    SoftwareSourcePrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOFTWARESOURCE_H
