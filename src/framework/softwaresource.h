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

class SoftwareSourcePrivate;

class LIRIAPPCENTER_EXPORT SoftwareSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *backend READ backend WRITE setBackend NOTIFY backendChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString section READ section WRITE setSection NOTIFY sectionChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(int priority READ priority WRITE setPriority NOTIFY priorityChanged)
    Q_DECLARE_PRIVATE(SoftwareSource)
    Q_DISABLE_COPY(SoftwareSource)
public:
    explicit SoftwareSource(QObject *parent = nullptr);
    ~SoftwareSource();

    QObject *backend() const;
    void setBackend(QObject *backend);

    QString name() const;
    void setName(const QString &name);

    QString title() const;
    void setTitle(const QString &title);

    QString section() const;
    void setSection(const QString &section);

    bool isEnabled() const;
    void setEnabled(bool flag);

    QUrl url() const;
    void setUrl(const QUrl &url);

    int priority() const;
    void setPriority(int prio);

Q_SIGNALS:
    void backendChanged();
    void nameChanged();
    void titleChanged();
    void sectionChanged();
    void enabledChanged();
    void urlChanged();
    void priorityChanged();

private:
    SoftwareSourcePrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOFTWARESOURCE_H
