// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSCREENSHOTSMODEL_H
#define LIRIAPPCENTERSCREENSHOTSMODEL_H

#include <QAbstractListModel>
#include <QUrl>

#include <LiriAppCenter/SoftwareResource>

namespace Liri {

namespace AppCenter {

class ScreenshotsModelPrivate;

class LIRIAPPCENTER_EXPORT ScreenshotsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(SoftwareResource *app READ app WRITE setApp NOTIFY appChanged)
    Q_PROPERTY(QSize maximumThumbnailSize READ maximumThumbnailSize NOTIFY appChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_DECLARE_PRIVATE(ScreenshotsModel)
    Q_DISABLE_COPY(ScreenshotsModel)
public:
    enum Roles {
        ThumbnailUrlRole = Qt::UserRole + 1,
        ThumbnailSizeRole,
        ScreenshotUrlRole,
        ScreenshotSizeRole
    };

    explicit ScreenshotsModel(QObject *parent = nullptr);
    ~ScreenshotsModel();

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    SoftwareResource *app() const;
    void setApp(SoftwareResource *app);

    QSize maximumThumbnailSize() const;

    Q_INVOKABLE QUrl thumbnailUrlAt(int index) const;
    Q_INVOKABLE QUrl screenshotUrlAt(int index) const;

Q_SIGNALS:
    void appChanged();
    void countChanged();

private:
    ScreenshotsModelPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSCREENSHOTSMODEL_H
