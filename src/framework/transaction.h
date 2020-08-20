// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERTRANSACTION_H
#define LIRIAPPCENTERTRANSACTION_H

#include <QObject>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class SoftwareResource;
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
        Preparing,
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
                         bool cancellable = true,
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

    int progress() const;
    void setProgress(int value);

    bool isVisible() const;

    Q_INVOKABLE virtual void run() = 0;
    Q_INVOKABLE virtual void cancel() = 0;

Q_SIGNALS:
    void statusChanged();
    void succeeded();
    void failed();
    void cancellableChanged();
    void progressChanged();
    void errorMessageChanged();

private:
    TransactionPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERTRANSACTION_H
