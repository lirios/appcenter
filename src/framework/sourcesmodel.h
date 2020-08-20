// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOURCESMODEL_H
#define LIRIAPPCENTERSOURCESMODEL_H

#include <QAbstractListModel>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class SoftwareSource;
class SourcesModelPrivate;

class LIRIAPPCENTER_EXPORT SourcesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SourcesModel)
    Q_DISABLE_COPY(SourcesModel)
public:
    enum Roles {
        SourceRole = Qt::UserRole + 1,
        SectionRole,
        NameRole,
        TitleRole,
        CommentRole,
        DescriptionRole,
        LabelRole,
        EnabledRole,
        GpgVerifyRole,
        UrlRole,
        HostNameRole,
        IconUrlRole,
        PriorityRole
    };

    explicit SourcesModel(QObject *parent = nullptr);
    ~SourcesModel();

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setItemRoleNames(const QHash<int,QByteArray> &roles);

    void addSource(SoftwareSource *source);
    void removeSource(SoftwareSource *source);

    SoftwareSource *findSource(const QString &name) const;

private:
    SourcesModelPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOURCESMODEL_H
