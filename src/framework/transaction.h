/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef LIRIAPPCENTERTRANSACTION_H
#define LIRIAPPCENTERTRANSACTION_H

#include <QObject>

#include <LiriAppCenter/SoftwareResource>

namespace Liri {

namespace AppCenter {

class TransactionPrivate;

class LIRIAPPCENTER_EXPORT Transaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(bool cancellable READ isCancellable NOTIFY cancellableChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool visible READ isVisible CONSTANT)
    Q_DECLARE_PRIVATE(Transaction)
    Q_DISABLE_COPY(Transaction)
public:
    enum Type {
        Sources,
        Metadata,
        Install,
        Uninstall,
        Update
    };
    Q_ENUM(Type)

    enum Status {
        Starting,
        Waiting,
        Downloading,
        Committing,
        Succeeded,
        Failed,
        Cancelled
    };
    Q_ENUM(Status)

    explicit Transaction(Type type,
                         const QString &name,
                         const QString &description,
                         SoftwareResource *resource,
                         QObject *parent = nullptr);
    ~Transaction();

    SoftwareResource *resource() const;

    Type type() const;

    QString name() const;
    QString description() const;

    Status status() const;
    void setStatus(Status status);

    QString errorMessage() const;
    void setErrorMessage(const QString &message);

    bool isCancellable() const;
    void setCancellable(bool value);

    int progress() const;
    void setProgress(int value);

    bool isVisible() const;

    Q_INVOKABLE virtual void run() = 0;
    Q_INVOKABLE virtual void cancel() = 0;

Q_SIGNALS:
    void statusChanged();
    void cancellableChanged();
    void progressChanged();
    void errorMessageChanged();

private:
    TransactionPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERTRANSACTION_H
